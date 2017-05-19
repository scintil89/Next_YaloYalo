#pragma once

#include <list>
#include <mutex>
#include <vector>

namespace LibNet
{
	class ClientSession;
	struct NetConfig;

	class SessionManager
	{
	public:
		~SessionManager();

		void PrepareSessions(NetConfig* config);
		bool AcceptSessions();

		ClientSession* GetSessionById(int id) { return _sessionList[id]; }

		void ReturnClientSession(ClientSession* client);

	private:
		using ClientList = std::list<ClientSession*>;
		ClientList _freeSessionList;
		std::vector<ClientSession*> _sessionList;

		NetConfig* _config = nullptr;
		std::mutex _mutex;

		uint64_t _currentIssueCount = 0;
		uint64_t _currentReturnCount = 0;
	};

	extern SessionManager* GSessionManager;
}
