#include "Match.h"

#include "UnitManager.h"
#include "WayManager.h"

#include "MatchMother.h"
#include "json/json.h"
#include "Packets.h"

#include "MapSpecs.h"
#include "Logger.h"

using namespace LibNet;
using namespace Packets;

namespace LibLogic
{
	using namespace DEF;

	Match::Match(std::pair<PlayerInfo, PlayerInfo> playerInfos, std::weak_ptr<BroadCaster> broadCaster, MatchMother* mother, int roomNum) :
		_broadCaster(broadCaster), _mother(mother), _roomNum(roomNum)
	{
		auto& pInfo1 = playerInfos.first;
		auto& pInfo2 = playerInfos.second;

		_players.emplace(PlayerSide::BOTTOM, Player{ pInfo1.first,pInfo1.second,PlayerSide::BOTTOM, 5 });
		_players.emplace(PlayerSide::TOP, Player{ pInfo2.first,pInfo2.second,PlayerSide::TOP, 5 });

		SendJoinSuccessPacket();

		_unitManager = std::make_unique<UnitManager>(this);
		_yaloManager = std::make_unique<YaloManager>(this, _players);
	}

	Match::~Match()
	{
		LibLogger::cInfo << "Match Object (RN: " << _roomNum << ") Destroyed" << std::endl;
	}

	void Match::SendJoinSuccessPacket()
	{
		auto& player1 = _players.at(PlayerSide::BOTTOM);
		auto& player2 = _players.at(PlayerSide::TOP);

		// send match success packet to players
		auto successPacket = PACKET_MATCH_JOIN_RES{};
		successPacket._isSuccess = true;
		// to player1
		successPacket._opponentNickname = player2._nickName;
		SendKing::SendPacket(player1._sessionId, ID_PACKET_MATCH_JOIN_RES, successPacket);
		// to player2
		successPacket._opponentNickname = player1._nickName;
		SendKing::SendPacket(player2._sessionId, ID_PACKET_MATCH_JOIN_RES, successPacket);
	}

	void Match::SendMatchStartPacket()
	{
		auto& player1 = _players.at(PlayerSide::BOTTOM);
		auto& player2 = _players.at(PlayerSide::TOP);

		PACKET_MATCH_START_NTF startPacket;

		//Set map size 
		startPacket._mapSizeX = MapSpecs::MAP_SIZE._x;
		startPacket._mapSizeY = MapSpecs::MAP_SIZE._y;
		
		//Set yalo
		startPacket._yaloChargeTime = _yaloManager->GetYaloChargeTime();
		startPacket._yaloStartNum = _yaloManager->GetStartYaloNumber();
		startPacket._maxYaloNum = _yaloManager->GetMaxYaloNumber();

		//Set Bridge 
		startPacket._leftBridgeRectX = MapSpecs::LEFT_BRIDGE_AREA._x;
		startPacket._leftBridgeRectY = MapSpecs::LEFT_BRIDGE_AREA._y;
		startPacket._rightBridgeRectX = MapSpecs::RIGHT_BRIDGE_AREA._x;
		startPacket._rightBridgeRectY = MapSpecs::RIGHT_BRIDGE_AREA._y;
		startPacket._bridgeWidth = MapSpecs::BRIDGE_SIZE._x;
		startPacket._bridgeHeight = MapSpecs::BRIDGE_SIZE._y;

		// to player1
		startPacket._playerSide = PlayerSide::BOTTOM;
		SendKing::SendPacket(player1._sessionId, ID_PACKET_MATCH_START_NTF, startPacket);
		// to player2
		startPacket._playerSide = PlayerSide::TOP;
		SendKing::SendPacket(player2._sessionId, ID_PACKET_MATCH_START_NTF, startPacket);
	}

	void Match::SubscribeInterestingSubjects()
	{
		if (auto& spt = _broadCaster.lock())
		{
			for (auto& player : _players)
			{
				auto SubscribeAndAddToList = [&](PacketId pktId) {
					auto subject = Subject{ pktId,player.second._sessionId };
					spt->Subscribe(subject, shared_from_this());
					_subjectList.push_back(subject);
				};
				
				// subscribe session close ntf
				SubscribeAndAddToList(ID_PACKET_CONNECT_CLOSE_NTF);

				// subscribe request packets
				SubscribeAndAddToList(ID_PACKET_UNIT_SUMMON_REQ);
				SubscribeAndAddToList(ID_PACKET_CLIENT_GAME_READY_REQ);
			}
		}
		else
		{
			// TODO: process error
			LibLogger::cError << "weak ptr is expired" << std::endl;
		}
	}

	void Match::Run()
	{
		auto& player1 = _players.at(PlayerSide::BOTTOM);
		auto& player2 = _players.at(PlayerSide::TOP);

		LibLogger::cInfo << "new match(RN: "<< _roomNum <<") starts! : " <<
			player1._nickName << " vs " << player2._nickName << std::endl;

		SubscribeInterestingSubjects();
		SendMatchStartPacket();

		_lastTime = std::chrono::system_clock::now();

		while (_stopFlag == false)
		{
			//서버 시간 처리
			auto dt = WaitForDeltaTime();

			//클라 패킷 처리
			ProcessPacketFromClient();

			//서버 로직 처리
			_unitManager->UpdateAllUnits(dt);

			//얄로 처리
			_yaloManager->Update(dt);
		}
	}

	float Match::WaitForDeltaTime()
	{
		//delta time을 계산한다
		using seconds = std::chrono::duration<float>;
		auto currentTime = std::chrono::system_clock::now();
		float dt = std::chrono::duration_cast<seconds>(currentTime - _lastTime).count();
		
		//tick rate만큼 기다린다
		if (dt < (1.f / _tickRate))
		{
			Sleep(((1.f / _tickRate) - dt) * 1000.f);
		}

		//기다린 시간을 반영한다
		currentTime = std::chrono::system_clock::now();
		dt = std::chrono::duration_cast<seconds>(currentTime - _lastTime).count();
		_lastTime = currentTime;
		
		return dt;
	}

	void Match::ProcessPacketFromClient()
	{
		while (_packetCount > 0)
		{
			auto& pktInfo = _recvPacketQueue->ReadFront();
			
			//Extract Packet Info
			auto& pktId = pktInfo->_packetId;
			auto playerSide = GetPlayerSideFromSessionIndex(pktInfo->_sessionIndex);
			auto& reader = Json::Reader();
			auto jVal = Json::Value();
			reader.parse(pktInfo->_data, pktInfo->_data + pktInfo->_packetBodySize, jVal);

			//Process packets
			switch (pktId)
			{
			case ID_PACKET_UNIT_SUMMON_REQ:
				ProcessSummonReq(jVal, playerSide);
				break;
			case ID_PACKET_CLIENT_GAME_READY_REQ:
				ProcessGameReady(jVal);
				break;
			case ID_PACKET_CONNECT_CLOSE_NTF:
				ProcessCloseSession(jVal, playerSide);
				break;
			}

			_recvPacketQueue->PopFront();
			_packetCount.fetch_sub(1);
		}
	}

	void Match::ProcessSummonReq(Json::Value& jVal, DEF::PlayerSide playerSide)
	{
		PACKET_UNIT_SUMMON_REQ pkt;
		pkt.Deserialize(jVal);

		int summonCost = _yaloManager->GetCost(pkt._unitName);

		if (_players.at(playerSide)._currentYalo >= summonCost)
		{
			auto summonSuccess = 
				_unitManager->SummonUnit(pkt._unitName, Vec2{ pkt._posX, pkt._posY }, playerSide);
			
			if (summonSuccess) {
				_yaloManager->UseYalo(playerSide, summonCost);
			}
		}
	}

	void Match::ProcessGameReady(Json::Value& jVal)
	{
		PACKET_CLIENT_GAME_READY_REQ packet;
		packet.Deserialize(jVal);

		auto& player1 = _players.at(PlayerSide::BOTTOM);
		auto& player2 = _players.at(PlayerSide::TOP);

		//바텀 플레이어가 게임레디를 보냈다
		if (packet._gameReady == PlayerSide::TOP)
		{
			//바텀 플레이어 게임레디 체크
			player1._gameReady = true;
		}
		//탑 플레이어가 게임레디를 보냈다
		else
		{
			//탑 플레이어 게임레디 체크
			player2._gameReady = true;
		}

		//둘다 게임레디가 체크되었다
		if (player1._gameReady == true &&
			player2._gameReady == true)
		{
			LibLogger::cInfo << "Game Start!" << std::endl;

			_yaloManager->StartUpdate();

			//각 플레이어 본진 소환
			_unitManager->SummonUnit("King", MapSpecs::TOP_KING_POS, PlayerSide::TOP);
			_unitManager->SummonUnit("King", MapSpecs::BOTTOM_KING_POS, PlayerSide::BOTTOM);

			//각 플레이어의 앞마당 소환
			_unitManager->SummonUnit("Nexus", MapSpecs::LEFT_TOP_BUILDING_POS, PlayerSide::TOP);
			_unitManager->SummonUnit("Nexus", MapSpecs::RIGHT_TOP_BUILDING_POS, PlayerSide::TOP);
			_unitManager->SummonUnit("Nexus", MapSpecs::LEFT_BOTTOM_BUILDING_POS, PlayerSide::BOTTOM);
			_unitManager->SummonUnit("Nexus", MapSpecs::RIGHT_BOTTOM_BUILDING_POS, PlayerSide::BOTTOM);
		}
	}

	void Match::ProcessCloseSession(Json::Value& jVal, DEF::PlayerSide closeSide)
	{
		PACKET_GAME_RESULT_NTF pkt;

		//탑이 나갔으면
		if (closeSide == PlayerSide::TOP)
		{
			//바텀이 승리 처리
			pkt._winPlayerSide = PlayerSide::BOTTOM;
			SendPacketToPlayer(PlayerSide::BOTTOM, ID_PACKET_GAME_RESULT_NTF, pkt);
		}
		//바텀이 나갔으면
		else
		{
			//탑이 승리 처리
			pkt._winPlayerSide = PlayerSide::TOP;
			SendPacketToPlayer(PlayerSide::TOP, ID_PACKET_GAME_RESULT_NTF, pkt);
		}

		//매치 종료
		EndMatch((PlayerSide)pkt._winPlayerSide);
	}

	void Match::EndMatch(DEF::PlayerSide winner)
	{
		_stopFlag = true;

		// 구독했던 것들 모두 해지한다.
		for (auto& subject : _subjectList)
		{
			auto spt = _broadCaster.lock();
			if (spt != nullptr)
			{
				spt->UnSubscribe(subject, shared_from_this());
			}
		}

		_mother->EndMatch(_roomNum);
	}

	LibLogic::DEF::PlayerSide Match::GetPlayerSideFromSessionIndex(int sessionId)
	{
		int topPlayerSessionId = _players.at(PlayerSide::TOP)._sessionId;
		return (sessionId == topPlayerSessionId) ? PlayerSide::TOP : PlayerSide::BOTTOM;
	}
}