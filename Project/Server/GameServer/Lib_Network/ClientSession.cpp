#include <ws2tcpip.h>
#include <cassert>
#include <string>

#include "SessionManager.h"
#include "IOCPManager.h"
#include "Logger.h"
#include "PacketQueue.h"
#include "LockGuard.h"
#include "NetConfig.h"
#include "..\..\..\Common\Packet\Packets.h"

#include "ClientSession.h"

namespace LibNet
{
	ClientSession::ClientSession(NetConfig* config, unsigned int id)
		:_config(config), _sessionId(id)
	{
		memset(&_clientAddr, 0, sizeof(SOCKADDR_IN));
		_socket = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

		_recvBuffer = new char[config->_maxSessionBufferSize];
		_sendBuffer = new char[config->_maxSessionBufferSize];


		for (int count = 0; config->_bucketSize * (count + 1) < config->_maxSessionBufferSize; ++count)
		{
			_sendIndexBucket.push_back(count);
		}
	}

	ClientSession::~ClientSession()
	{
		delete[] _recvBuffer;
		delete[] _sendBuffer;
	}

	void ClientSession::SessionReset()
	{
		_connected = 0;
		_refCount = 0;
		memset(&_clientAddr, 0, sizeof(SOCKADDR_IN));

		ClearBuffers();

		LINGER lingerOption;
		lingerOption.l_onoff = 1;
		lingerOption.l_linger = 0;

		// on TCP TIME_WAIT
		if (SOCKET_ERROR == setsockopt(_socket, SOL_SOCKET, SO_LINGER, (char*)&lingerOption, sizeof(LINGER)))
		{
			LibLogger::cError << "[DEBUG] setsockopt linger option error : " << GetLastError() << std::endl;
		}
		closesocket(_socket);

		_socket = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	}

	int ClientSession::CopySendData(PacketHeader * header, char * data)
	{
		LockGuard lock(_mutex);

		if (_sendIndexBucket.empty())
		{
			LibLogger::cError << "Client send buffer is full! sID : " << _sessionId << std::endl;
			return -1;
		}

		if ((unsigned)header->_bodySize + PACKET_HEADER_SIZE > _config->_bucketSize)
		{
			LibLogger::cError << "Client send packet is too big!!! sID : " << _sessionId << std::endl;
			return -1;
		}

		int bucketIndex = _sendIndexBucket.back();
		_sendIndexBucket.pop_back();

		// copy header
		memcpy_s(_sendBuffer + (bucketIndex * _config->_bucketSize), _config->_bucketSize, header, PACKET_HEADER_SIZE);

		// body
		memcpy_s(_sendBuffer + (bucketIndex * _config->_bucketSize) + PACKET_HEADER_SIZE, _config->_maxSessionBufferSize - PACKET_HEADER_SIZE, data, header->_bodySize);

		return bucketIndex;
	}

	bool ClientSession::PostAccept()
	{
		OverlappedAcceptContext* acceptContext = new OverlappedAcceptContext(this);
		DWORD bytes = 0;
		DWORD flags = 0;
		acceptContext->_wsaBuf.len = 0;
		acceptContext->_wsaBuf.buf = nullptr;

		auto iocpMgr = IOCPManager::GetInstance();

		if (FALSE == IOCPManager::GetInstance()->AcceptEx(*(iocpMgr->GetListenSocket()), _socket, iocpMgr->_acceptBuf, 0,
			sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &bytes, (LPOVERLAPPED)acceptContext))
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				DeleteIoContext(acceptContext);
				LibLogger::cError << "AcceptEx Error : " << GetLastError() << std::endl;
				return false;
			}
		}

		return true;
	}

	void ClientSession::AcceptCompletion()
	{
		if (1 == InterlockedExchange(&_connected, 1))
		{
			assert(false);
			return;
		}

		bool resultOk = true;
		do
		{
			auto IocpMgr = IOCPManager::GetInstance();
			if (SOCKET_ERROR == setsockopt(_socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)IocpMgr->GetListenSocket(), sizeof(SOCKET)))
			{
				LibLogger::cError << "SO_UPDATE_ACCEPT_CONTEXT error : " << GetLastError() << std::endl;
				resultOk = false;
				break;
			}

			int opt = 1;
			if (SOCKET_ERROR == setsockopt(_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(int)))
			{
				LibLogger::cError << "TCP_NODELAY error : " << GetLastError() << std::endl;
				resultOk = false;
				break;
			}

			opt = 0;
			if (SOCKET_ERROR == setsockopt(_socket, SOL_SOCKET, SO_RCVBUF, (const char*)&opt, sizeof(int)))
			{
				LibLogger::cError << "SO_RCVBUF change error : " << GetLastError() << std::endl;
				resultOk = false;
				break;
			}

			int addrlen = sizeof(SOCKADDR_IN);
			if (SOCKET_ERROR == getpeername(_socket, (SOCKADDR*)&_clientAddr, &addrlen))
			{
				LibLogger::cError << "getpeername error : " << GetLastError() << std::endl;
				resultOk = false;
				break;
			}

			HANDLE handle = CreateIoCompletionPort((HANDLE)_socket, IocpMgr->GetCompletionPort(), (ULONG_PTR)this, 0);
			if (handle != IocpMgr->GetCompletionPort())
			{
				LibLogger::cError << "CreateIoCompletionPort error : " << GetLastError() << std::endl;
				resultOk = false;
				break;
			}
		} while (false);

		if (!resultOk)
		{
			DisconnectRequest(DR_ONCONNECT_ERROR);
			return;
		}
		char ip[INET_ADDRSTRLEN];
		InetNtop(AF_INET, &_clientAddr.sin_addr, ip, INET_ADDRSTRLEN);
		LibLogger::cInfo << "Client Connected IP=" << ip << ", PORT=" << std::to_string(ntohs(_clientAddr.sin_port)) << std::endl;

		if (false == PreRecv())
		{
			LibLogger::cError << "PreRecv error : " << GetLastError() << std::endl;
		}
	}

	bool ClientSession::PreRecv()
	{
		if (!IsConnected())
		{
			return false;
		}

		OverlappedPreRecvContext* recvContext = new OverlappedPreRecvContext(this);

		DWORD recvbytes = 0;
		DWORD flags = 0;
		recvContext->_wsaBuf.len = 0;
		recvContext->_wsaBuf.buf = nullptr;

		// start async recv
		if (SOCKET_ERROR == WSARecv(_socket, &recvContext->_wsaBuf, 1, &recvbytes, &flags, (LPWSAOVERLAPPED)recvContext, NULL))
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				DeleteIoContext(recvContext);
				LibLogger::cError << "ClientSession::PreRecv error : " << GetLastError();
				return false;
			}
		}

		return true;
	}

	bool ClientSession::PostRecv()
	{
		if (!IsConnected())
		{
			return false;
		}

		LockGuard criticalSection(_mutex);

		if (_curRecvBufferSize >= _config->_maxSessionBufferSize)
		{
			return false;
		}

		OverlappedRecvContext* recvContext = new OverlappedRecvContext(this);

		DWORD recvBytes = 0;
		DWORD flags = 0;
		recvContext->_wsaBuf.len = (ULONG)(_config->_maxSessionBufferSize - _curRecvBufferSize);
		recvContext->_wsaBuf.buf = (CHAR*)_recvBuffer + _curRecvBufferSize;

		if (SOCKET_ERROR == WSARecv(_socket, &recvContext->_wsaBuf, 1, &recvBytes, &flags, (LPWSAOVERLAPPED)recvContext, NULL))
		{
			auto aa = WSAGetLastError();
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				DeleteIoContext(recvContext);
				if(GetLastError() != 10057)
					LibLogger::cError << "ClientSession::PostRecv Error : " << GetLastError() << std::endl;
				return false;
			}
		}

		return true;
	}

	void ClientSession::RecvCompletion(DWORD transferred)
	{
		if (!IsConnected())
		{
			return;
		}

		// Total data size to be processed
		unsigned int totalSize = transferred + _curRecvBufferSize;
		unsigned int readPos = 0;

		LockGuard criticalSection(_mutex);

		while (totalSize >= PACKET_HEADER_SIZE)
		{
			PacketHeader* header = (PacketHeader*)_recvBuffer + readPos;
			if ((unsigned)PACKET_HEADER_SIZE + header->_bodySize >= totalSize)
			{
				// 복사 해야 하는데 안하고 있던 부분 복사하게 고쳤습니다. - 석민
				auto copiedData = new char[header->_bodySize];
				memcpy(copiedData, _recvBuffer + readPos + PACKET_HEADER_SIZE, header->_bodySize);
				auto newPacket = std::make_shared<PacketInfo>(_sessionId, header->_id, header->_bodySize, copiedData,true);
				// The packet will be copied
				IOCPManager::GetInstance()->GetRecvPacketQueue()->PushBack(newPacket);

				readPos += PACKET_HEADER_SIZE + header->_bodySize;
				totalSize -= PACKET_HEADER_SIZE + header->_bodySize;
			}
		}

		// After Creating packet, move the remaining data to front
		memcpy_s(_recvBuffer, _config->_maxSessionBufferSize, _recvBuffer + readPos, totalSize);
		_curRecvBufferSize = totalSize;
		
		// Set zero byte recv
		PreRecv();
	}

	bool ClientSession::PostSend(int bucketIndex, unsigned packetSize)
	{
		if (!IsConnected())
		{
			return false;
		}

		LockGuard criticalSection(_mutex);

		OverlappedSendContext* sendContext = new OverlappedSendContext(this, bucketIndex);

		DWORD sendBytes = 0;
		DWORD flags = 0;
		sendContext->_wsaBuf.len = packetSize;
		sendContext->_wsaBuf.buf = (CHAR*)_sendBuffer + (_config->_bucketSize * bucketIndex);

		if (SOCKET_ERROR == WSASend(_socket, &sendContext->_wsaBuf, 1, &sendBytes, flags, (LPWSAOVERLAPPED)sendContext, NULL))
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				DeleteIoContext(sendContext);
				LibLogger::cError << "ClientSession::PostSend Error : " << GetLastError() << std::endl;

				return false;
			}
		}
		
		return true;
	}

	void ClientSession::SendCompletion(char* buf, DWORD transferred, int bucketIndex)
	{
		LockGuard criticalSection(_mutex);

		// DELETE :: move the rest of the data after buf + transferred to buf's position
		//memcpy_s(buf, _config->_maxSessionBufferSize - (buf - _sendBuffer), buf + transferred, _curSendBufferSize - (buf - _sendBuffer) - transferred);

		// return the index to the bucket to be reused
		_sendIndexBucket.push_back(bucketIndex);
	}

	void ClientSession::DisconnectRequest(DisconnectReason dr)
	{
		// if already disconnected
		if (0 == InterlockedExchange(&_connected, 0))
			return;
		
		OverlappedDisconnectContext* context = new OverlappedDisconnectContext(this, dr);

		if (FALSE == IOCPManager::GetInstance()->DisconnectEx(_socket, (LPWSAOVERLAPPED)context, TF_REUSE_SOCKET, 0))
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				DeleteIoContext(context);
				LibLogger::cError << "ClientSession::DisconnectRequest Error : " << GetLastError() << std::endl;
			}
		}
	}

	void ClientSession::DisconnectCompletion(DisconnectReason dr)
	{
		//접속종료 패킷 넣기
		auto dataSize = sizeof(Packets::PACKET_CONNECT_CLOSE_NTF);
		auto copiedData = new char[dataSize];
		memcpy(copiedData, &_sessionId, dataSize);
		auto newPktInfo = std::make_shared<PacketInfo>(_sessionId, Packets::ID_PACKET_CONNECT_CLOSE_NTF, dataSize, copiedData, true);
		
		IOCPManager::GetInstance()->GetRecvPacketQueue()->PushBack(newPktInfo);

		LibLogger::cDebug << "Client Disconnected : Reason=" << dr << ", PORT=" << ntohs(_clientAddr.sin_port) << std::endl;
		
		// release ref count
		ReleaseRef();
	}

	void ClientSession::AddRef()
	{
		assert(InterlockedIncrement(&_refCount) > 0);
	}

	void ClientSession::ReleaseRef()
	{
		long ret = InterlockedDecrement(&_refCount);
		assert(ret >= 0);

		if (ret == 0)
		{
			GSessionManager->ReturnClientSession(this);
			//GSessionManager->AcceptSessions();
		}
	}

	void ClientSession::ClearBuffers()
	{
		_recvBuffer = _recvBuffer - _curRecvBufferSize;
		_curRecvBufferSize = 0;
	}

	OverlappedIOContext::OverlappedIOContext(ClientSession * owner, IOType ioType)
		:_sessionObject(owner), _ioType(ioType)
	{
		memset(&_overlapped, 0, sizeof(OVERLAPPED));
		memset(&_wsaBuf, 0, sizeof(WSABUF));
		//_sessionObject->AddRef();
	}

	void DeleteIoContext(OverlappedIOContext * context)
	{
		if (context == nullptr)
		{
			return;
		}

		//context->_sessionObject->ReleaseRef();

		switch (context->_ioType)
		{
		case IO_SEND:
			delete static_cast<OverlappedSendContext*>(context);
			break;

		case IO_RECV_ZERO:
			delete static_cast<OverlappedPreRecvContext*>(context);
			break;

		case IO_RECV:
			delete static_cast<OverlappedRecvContext*>(context);
			break;
		case IO_DISCONNECT:
			delete static_cast<OverlappedDisconnectContext*>(context);
			break;
		case IO_ACCEPT:
			delete static_cast<OverlappedAcceptContext*>(context);
			break;
			
		default:
			assert(false);
		}
	}

}
