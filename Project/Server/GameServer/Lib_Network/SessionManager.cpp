#include <cassert>

#include "ClientSession.h"
#include "LockGuard.h"

#include "SessionManager.h"
#include "NetConfig.h"

namespace LibNet
{
	SessionManager* GSessionManager = nullptr;

	SessionManager::~SessionManager()
	{
		_sessionList.clear();

		for (auto it : _freeSessionList)
		{
			delete(it);
		}

		_freeSessionList.clear();
	}

	void SessionManager::PrepareSessions(NetConfig* config)
	{
		if (config == nullptr)
		{
			return;
		}

		_config = config;

		for (unsigned int i = 0; i < _config->_maxSessionNum; ++i)
		{
			ClientSession* client = new ClientSession(_config, i);

			_freeSessionList.push_back(client);
			_sessionList.push_back(client);
		}
	}

	bool SessionManager::AcceptSessions()
	{
		LockGuard criticalSection(_mutex);

		while (_currentIssueCount - _currentReturnCount < _config->_maxSessionNum)
		{
			ClientSession* newClient = _freeSessionList.back();
			_freeSessionList.pop_back();

			++_currentIssueCount;

			newClient->AddRef(); // refcount +1 for issuing

			if (false == newClient->PostAccept())
			{
				return false;
			}
		}

		return true;
	}

	void SessionManager::ReturnClientSession(ClientSession * client)
	{
		LockGuard criticalSection(_mutex);

		assert(client->_connected == 0 && client->_refCount == 0);

		client->SessionReset();

		_freeSessionList.push_back(client);

		++_currentReturnCount;
	}

}