#pragma once

#include <mutex>
#include <deque>
#include <WinSock2.h>

#include "NetPacketInfo.h"


namespace LibNet
{
	class ClientSession;
	struct NetConfig;

	enum IOType
	{
		IO_NONE,
		IO_SEND,
		IO_RECV,
		IO_RECV_ZERO,
		IO_ACCEPT,
		IO_DISCONNECT
	};

	enum DisconnectReason
	{
		DR_NONE,
		DR_ACTIVE,
		DR_ONCONNECT_ERROR,
		DR_IO_REQUEST_ERROR,
		DR_COMPLETION_ERROR,
	};

	struct OverlappedIOContext
	{
		OverlappedIOContext(ClientSession* owner, IOType ioType);
		
		OVERLAPPED		_overlapped;
		ClientSession*	_sessionObject;
		IOType			_ioType;
		WSABUF			_wsaBuf;
	};

	struct OverlappedSendContext : public OverlappedIOContext
	{
		OverlappedSendContext(ClientSession* owner, unsigned bucketIndex) : OverlappedIOContext(owner, IO_SEND), _bucketIndex(bucketIndex)
		{
		}

		unsigned _bucketIndex;
	};

	struct OverlappedRecvContext : public OverlappedIOContext
	{
		OverlappedRecvContext(ClientSession* owner) : OverlappedIOContext(owner, IO_RECV)
		{
		}
	};

	struct OverlappedPreRecvContext : public OverlappedIOContext
	{
		OverlappedPreRecvContext(ClientSession* owner) : OverlappedIOContext(owner, IO_RECV_ZERO)
		{
		}
	};

	struct OverlappedDisconnectContext : public OverlappedIOContext
	{
		OverlappedDisconnectContext(ClientSession* owner, DisconnectReason dr) 
			: OverlappedIOContext(owner, IO_DISCONNECT), _disConnectReason(dr)
		{
		}

		DisconnectReason _disConnectReason;
	};

	struct OverlappedAcceptContext : public OverlappedIOContext
	{
		OverlappedAcceptContext(ClientSession* owner) : OverlappedIOContext(owner, IO_ACCEPT)
		{
		}
	};

	void DeleteIoContext(OverlappedIOContext* context);

	class ClientSession
	{
	public:
		ClientSession(NetConfig* config, unsigned int id);
		~ClientSession();

		void	SessionReset();

		bool	IsConnected() const { return !!_connected; }

		int		CopySendData(PacketHeader* header, char* data);

		bool	PostAccept();
		void	AcceptCompletion();

		bool	PreRecv(); // zero byte recv

		bool	PostRecv();
		void	RecvCompletion(DWORD transferred);

		bool	PostSend(int bucketIndex, unsigned packetSize);
		void	SendCompletion(char* buf, DWORD transferred, int bucketIndex);

		void	DisconnectRequest(DisconnectReason dr);
		void	DisconnectCompletion(DisconnectReason dr);

		void	AddRef();
		void	ReleaseRef();
		
	private:
		void	ClearBuffers();

	private:

		int		_sessionId;

		NetConfig*			_config = nullptr;

		SOCKET				_socket;
		SOCKADDR_IN			_clientAddr;

		std::mutex			_mutex;

		char*				_recvBuffer = nullptr;
		unsigned int		_curRecvBufferSize = 0;
		char*				_sendBuffer = nullptr;
		std::deque<int>		_sendIndexBucket;

		volatile long		_refCount = 0;
		volatile long		_connected = 0;

		friend class SessionManager;
	};

}

