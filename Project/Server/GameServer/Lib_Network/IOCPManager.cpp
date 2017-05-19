#include <ws2tcpip.h>
#include <cassert>

#include "PacketQueue.h"
#include "ClientSession.h"
#include "SessionManager.h"
#include "Logger.h"

#include "IOCPManager.h"

namespace LibNet
{
	IOCPManager* IOCPManager::_instance = nullptr;

	char IOCPManager::_acceptBuf[64] = { 0, };
	LPFN_DISCONNECTEX IOCPManager::_fnDisconnectEx = nullptr;
	LPFN_ACCEPTEX IOCPManager::_fnAcceptEx = nullptr;

	IOCPManager* IOCPManager::GetInstance()
	{
		if (_instance == nullptr)
		{
			_instance = new IOCPManager();
		}
		return _instance;
	}

	void IOCPManager::Run()
	{
		Initialize();
		StartIoThreads();
		auto& accepThread = std::thread(std::bind(&IOCPManager::StartAccept, this));
		accepThread.detach();
	}

	IOCPManager::IOCPManager()
	{
		_recvPacketQueue = std::make_shared<PacketQueue>();
	}

	bool IOCPManager::Initialize()
	{
		LoadConfig();

		GSessionManager = new SessionManager();

		// set num of I/O threads
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		_ioThreadCount = si.dwNumberOfProcessors;

		// Initialize Winsock
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR)
			return false;

		// Create a handle for the completion port
		_completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
		if (_completionPort == NULL)
			return false;

		// Create a listening socket
		_listenSocket = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (_listenSocket == INVALID_SOCKET)
		{
			return false;
		}

		// Associate the listening socket with the completion port
		HANDLE handle = CreateIoCompletionPort((HANDLE)_listenSocket, _completionPort, 0, 0);
		if (handle != _completionPort)
		{
			return false;
		}

		// AcceptEx()를 사용할 경우, listen() 에서 자동적으로 accept를 받지 못하도록 한다.
		int opt = 1;
		setsockopt(_listenSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(int));

		// Bind the listening socket to the IP address and server port
		sockaddr_in service;
		ZeroMemory(&service, sizeof(service));
		service.sin_family = AF_INET;
		InetPton(AF_INET, INADDR_ANY, &service.sin_addr);
		service.sin_port = htons(_config._port);

		if (SOCKET_ERROR == bind(_listenSocket, (SOCKADDR*)&service, sizeof(service)))
		{
			return false;
		}

		GUID guidDisconnectEx = WSAID_DISCONNECTEX;
		DWORD bytes = 0;
		if (SOCKET_ERROR == WSAIoctl(_listenSocket, SIO_GET_EXTENSION_FUNCTION_POINTER,
			&guidDisconnectEx, sizeof(GUID), &_fnDisconnectEx, sizeof(LPFN_DISCONNECTEX), &bytes, NULL, NULL))
		{
			return false;
		}

		GUID guidAcceptEx = WSAID_ACCEPTEX;
		if (SOCKET_ERROR == WSAIoctl(_listenSocket, SIO_GET_EXTENSION_FUNCTION_POINTER,
			&guidAcceptEx, sizeof(GUID), &_fnAcceptEx, sizeof(LPFN_ACCEPTEX), &bytes, NULL, NULL))
		{
			return false;
		}

		GSessionManager->PrepareSessions(&_config);

		return true;
	}

	bool IOCPManager::StartIoThreads()
	{
		// I/O 스레드 생성
		for (int i = 0; i < _ioThreadCount; ++i)
		{
			auto workerThread = std::thread(std::bind(&IOCPManager::IoWorkerThread, this));
			workerThread.detach();
		}

		return true;
	}

	void IOCPManager::StartAccept()
	{
		// listen
		if (SOCKET_ERROR == listen(_listenSocket, _config._backLog))
		{
			printf_s("[DEBUG] listen error\n");
			return;
		}

		while (GSessionManager->AcceptSessions())
		{
			Sleep(100);
		}
	}

	bool IOCPManager::PostSend(std::shared_ptr<PacketInfo> packet)
	{
		auto client = GSessionManager->GetSessionById(packet->_sessionIndex);

		PacketHeader header;
		header._id = packet->_packetId;
		header._bodySize = packet->_packetBodySize;

		int bucketIndex = client->CopySendData(&header, packet->_data);

		if (0 > bucketIndex)
		{
			return false;
		}

		client->PostSend(bucketIndex, PACKET_HEADER_SIZE + header._bodySize);

		return true;
	}

	BOOL IOCPManager::DisconnectEx(SOCKET hSocket, LPOVERLAPPED lpOverlapped, DWORD dwFlags, DWORD reserved)
	{
		return _fnDisconnectEx(hSocket, lpOverlapped, dwFlags, reserved);
	}

	BOOL IOCPManager::AcceptEx(SOCKET sListenSocket, SOCKET sAcceptSocket, PVOID lpOutputBuffer, DWORD dwReceiveDataLength, DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, LPDWORD lpdwBytesReceived, LPOVERLAPPED lpOverlapped)
	{
		return _fnAcceptEx(sListenSocket, sAcceptSocket, lpOutputBuffer, dwReceiveDataLength,
			dwLocalAddressLength, dwRemoteAddressLength, lpdwBytesReceived, lpOverlapped);
	}

	void IOCPManager::LoadConfig()
	{
		strcpy_s(_config._ip, "127.0.0.1");
		_config._port = 34343;
		_config._maxSessionNum = 1000;
		_config._maxSessionBufferSize = 1024000;
		_config._backLog = 100;
		_config._clientBufferSize = 4096;
		_config._bucketSize = 1024;
	}

	void IOCPManager::IoWorkerThread()
	{
		HANDLE hCompletionPort = _completionPort;

 		while (true)
		{
			DWORD dwTransferred = 0;
			OverlappedIOContext* context = nullptr;
			ULONG_PTR completionKey = 0;

			int ret = GetQueuedCompletionStatus(hCompletionPort, &dwTransferred, (PULONG_PTR)&completionKey, (LPOVERLAPPED*)&context, INFINITE);

			ClientSession* theClient = context ? context->_sessionObject : nullptr;

			if (ret == 0 || dwTransferred == 0)
			{
				int gle = GetLastError();

				if (gle == WAIT_TIMEOUT)
				{
					continue;
				}

				if (context->_ioType == IO_RECV || context->_ioType == IO_SEND)
				{
					assert(theClient != nullptr);

					theClient->DisconnectRequest(DR_COMPLETION_ERROR);

					DeleteIoContext(context);

					continue;
				}
			}

			assert(theClient != nullptr);

			bool completionOk = false;
			switch (context->_ioType)
			{
			case IO_DISCONNECT:
				theClient->DisconnectCompletion(static_cast<OverlappedDisconnectContext*>(context)->_disConnectReason);
				completionOk = true;
				break;

			case IO_ACCEPT:
				theClient->AcceptCompletion();
				completionOk = true;
				break;

			case IO_RECV_ZERO:
				completionOk = PreReceiveCompletion(theClient, static_cast<OverlappedPreRecvContext*>(context), dwTransferred);
				break;

			case IO_SEND:
				completionOk = SendCompletion(theClient, static_cast<OverlappedSendContext*>(context), dwTransferred);
				break;

			case IO_RECV:
				completionOk = ReceiveCompletion(theClient, static_cast<OverlappedRecvContext*>(context), dwTransferred);
				break;

			default:
				printf_s("Unknown I/O Type: %d\n", context->_ioType);
				assert(false);
				break;
			}

			if (!completionOk)
			{
				theClient->DisconnectRequest(DR_IO_REQUEST_ERROR);
			}

			DeleteIoContext(context);
		}

		return;
	}

	bool IOCPManager::PreReceiveCompletion(ClientSession * client, OverlappedPreRecvContext * context, DWORD dwTransferred)
	{
		// Real recieve
		return client->PostRecv();
	}

	bool IOCPManager::ReceiveCompletion(ClientSession * client, OverlappedRecvContext * context, DWORD dwTransferred)
	{
		client->RecvCompletion(dwTransferred);

		return true;
	}

	bool IOCPManager::SendCompletion(ClientSession * client, OverlappedSendContext * context, DWORD dwTransferred)
	{
		client->SendCompletion((char*)context->_wsaBuf.buf, dwTransferred, context->_bucketIndex);

		if (context->_wsaBuf.len != dwTransferred)
		{
			LibLogger::cError << "Partial SendCompletion requested [" << context->_wsaBuf.len << "] sent [" << dwTransferred << "]" << std::endl;
			return false;
		}

		// zero byte recv
		return client->PreRecv();
	}
}