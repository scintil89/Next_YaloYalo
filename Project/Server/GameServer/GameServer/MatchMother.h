#pragma once

#include <memory>

#include "Observer.h"
#include "Match.h"

namespace LibNet
{
	class PacketQueue;
}

namespace LibLogic
{
	class BroadCaster;
	class MatchMother : public Observer
	{
	public:
		MatchMother(std::weak_ptr<BroadCaster> broadCaster);
		~MatchMother() {}

		void Run();
		void Stop()
		{
			_stopFlag = true;
		}

		// TODO: need 'start match' function here
		void StartMatch(std::pair<PlayerInfo, PlayerInfo> sessionIds, int roomNum);

		void EndMatch(int roomNum);

	private:
		void Init();


	public:

	private:
		bool				_stopFlag = false;
		// users in each room
		// std::map<roomNum, Player>
		std::map<int,PlayerInfo>	_waitingPlayerInRooms;

		std::map<int,std::shared_ptr<Match>> _matchList;

		std::weak_ptr<BroadCaster> _broadCaster;
	};
}