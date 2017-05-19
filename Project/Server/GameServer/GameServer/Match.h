#pragma once
#include <map>
#include <list>
#include <memory>

#include "Observer.h"
#include "SendKing.h"
#include "BroadCaster.h"
#include "PacketQueue.h"

#include "UnitManager.h"
#include "YaloManager.h"

#include "Player.h"
#include "Define.h"

using namespace Packets;
namespace Packets
{
	enum PacketId;
}

namespace LibLogic
{
	class BroadCaster;
	class MatchMother;

	// session id, nickname
	using PlayerInfo = std::pair<int, std::string>;

	class Match : public Observer
	{
	public:
		Match(std::pair<PlayerInfo, PlayerInfo> playerInfos, std::weak_ptr<BroadCaster> broadCaster, MatchMother* mother, int roomNum);
		~Match();

		void Run();
		
		void EndMatch(DEF::PlayerSide winner);

		template <class T>
		void SendPacketToPlayer(DEF::PlayerSide playerSide, Packets::PacketId packetId, T& packet);

	private:
		//Setups
		void SubscribeInterestingSubjects();
		void SendJoinSuccessPacket();
		void SendMatchStartPacket();
		
		//Updates
		void ProcessPacketFromClient();
		void ProcessGameReady(Json::Value& jVal);
		void ProcessCloseSession(Json::Value& jVal, DEF::PlayerSide closeSide);
		void ProcessSummonReq(Json::Value& jVal, DEF::PlayerSide playerSide);
		
		//Utilities
		float WaitForDeltaTime();
		DEF::PlayerSide GetPlayerSideFromSessionIndex(int sessionId);
		
	public:
		LibNet::PacketQueue			_packetQueue;

	private:
		MatchMother*							_mother = nullptr;
		int										_roomNum = 0;
		bool									_stopFlag = false;

		//Server Time
		std::chrono::system_clock::time_point	_lastTime;
		float									_tickRate = 60.f;
		
		//Server Logic
		std::map<DEF::PlayerSide, Player>		_players;
		std::unique_ptr<UnitManager>			_unitManager;
		std::unique_ptr<YaloManager>			_yaloManager;

		//Network
		std::weak_ptr<BroadCaster>				_broadCaster;
		std::list<Subject>						_subjectList;
	};

	template <class T>
	void Match::SendPacketToPlayer(DEF::PlayerSide playerSide, Packets::PacketId packetId, T& packet)
	{
		SendKing::SendPacket(_players.at(playerSide)._sessionId, packetId, packet);
	}
}