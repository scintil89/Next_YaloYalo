#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include <mutex>
#include <winsock2.h>
#include <mswsock.h>

#include "NetErrorCode.h"
#include "NetConfig.h"
#include "NetPacketInfo.h"

namespace LibNet
{
	class PacketQueue;
	class ClientSession;
	struct OverlappedSendContext;
	struct OverlappedPreRecvContext;
	struct OverlappedRecvContext;

	class IOCPManager
	{
	public:
		static IOCPManager*		GetInstance();

		void					Run();


		std::shared_ptr<PacketQueue>	GetRecvPacketQueue() { return _recvPacketQueue; }

		bool PostSend(std::shared_ptr<PacketInfo> packet);
		
		BOOL DisconnectEx(SOCKET hSocket, LPOVERLAPPED lpOverlapped, DWORD dwFlags, DWORD reserved);
		BOOL AcceptEx(SOCKET sListenSocket, SOCKET sAcceptSocket, PVOID lpOutputBuffer, DWORD dwReceiveDataLength,
			DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, LPDWORD lpdwBytesReceived, LPOVERLAPPED lpOverlapped);

	private:
		IOCPManager();

		bool					Initialize();

		bool					StartIoThreads();
		void					StartAccept();

		void					LoadConfig();

		void					IoWorkerThread();

		SOCKET*					GetListenSocket() { return &_listenSocket; }
		HANDLE					GetCompletionPort() { return _completionPort; }

		static bool PreReceiveCompletion(ClientSession* client, OverlappedPreRecvContext* context, DWORD dwTransferred);
		static bool ReceiveCompletion(ClientSession* client, OverlappedRecvContext* context, DWORD dwTransferred);
		static bool SendCompletion(ClientSession* client, OverlappedSendContext* context, DWORD dwTransferred);

	public:
		static char					_acceptBuf[64];
		static LPFN_DISCONNECTEX	_fnDisconnectEx;
		static LPFN_ACCEPTEX		_fnAcceptEx;

	private:
		static IOCPManager*				_instance; // singleton
		int								_ioThreadCount = 4;

		std::shared_ptr<PacketQueue>	_recvPacketQueue = nullptr;
		NetConfig						_config;

		std::mutex						_sessionPoolMutex;

		HANDLE							_completionPort = INVALID_HANDLE_VALUE;
		SOCKET							_listenSocket = INVALID_SOCKET;
		//SOCKET							_acceptSocket = INVALID_SOCKET;

		friend class ClientSession;
	};

}