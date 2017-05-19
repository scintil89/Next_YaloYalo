#include "json/json.h"

#include "Packets.h"
#include "Logger.h"
#include "PacketQueue.h"

#include "BroadCaster.h"

#include "MatchMother.h"

namespace LibLogic
{

	MatchMother::MatchMother(std::weak_ptr<BroadCaster> broadCaster)
	{
		_broadCaster = broadCaster;
	}

	void MatchMother::Init()
	{
		// match mother is interested in match request packet
		// session id -1 means all sessions
		if (auto& spt = _broadCaster.lock())
		{
			spt->Subscribe(Subject{ Packets::ID_PACKET_MATCH_JOIN_REQ,ALL_SESSION }, shared_from_this());
		}
		else
		{
			LibLogger::cError << "weak ptr is expired" << std::endl;
		}
	}

	void MatchMother::Run()
	{
		Init();
		auto reader = Json::Reader();
		while (_stopFlag == false)
		{
			// If there is no packet to process, concede CPU to another thread
			if (_packetCount == 0)
			{
				Sleep(0);
				continue;
			}
			// lambda function to clean up used packet
			auto popUsedPacket = [&]{
				_recvPacketQueue->PopFront();
				_packetCount.fetch_sub(1);
			};

			// Arriving here means that we have least 1 packet to process
			// When match request packet arrived, start new match
			auto packetInfo = _recvPacketQueue->ReadFront();

			// case : match request packet
			if (packetInfo->_packetId == Packets::ID_PACKET_MATCH_JOIN_REQ)
			{
				auto& sessionId = packetInfo->_sessionIndex;
				auto& dataPos = packetInfo->_data;
				auto& bodySize = packetInfo->_packetBodySize;
				
				auto jsonData = Json::Value();
				reader.parse(dataPos, dataPos + bodySize, jsonData);
				
				Packets::PACKET_MATCH_JOIN_REQ packet;
				packet.Deserialize(jsonData);

				// process errors
				if (packet._nickname == "")
				{
					LibLogger::cError << "incorrect match request from user id(" << sessionId << ") : nickname is null" << std::endl;
					// TODO: error process
				}
				if (packet._roomNum < 0)
				{
					LibLogger::cError << "incorrect match request from user id(" << sessionId << ") : wrong room number (" << packet._roomNum << ")" << std::endl;
					// TODO: error process
				}
				if (packet._uuid == "")
				{
					LibLogger::cError << "incorrect match request from user id(" << sessionId << ") : uuid is null" << std::endl;
					// TODO: error process
				}

				if (true) // TODO: 지금 요청하는놈이 검증되었는지를 매치서버에 물어본다. 일단 true로 해놓음
				{
					// 아직 아무도 안왔을 때
					if (_waitingPlayerInRooms.find(packet._roomNum) == _waitingPlayerInRooms.end())
					{
						auto player1 = PlayerInfo{ sessionId,packet._nickname};
						_waitingPlayerInRooms.emplace(packet._roomNum, player1);
						LibLogger::cInfo << "player [" << packet._nickname << "] is waiting for opponent..." << std::endl;
					}
					// 이미 누가 기다리고 있을 때 ( 매치 성사 )
					else
					{
						// 룸 만든다
						auto& player1 = _waitingPlayerInRooms.at(packet._roomNum);
						auto player2 = PlayerInfo{ sessionId,packet._nickname };
						StartMatch(std::make_pair(player1, player2), packet._roomNum);
						// 매치 시작시켰으므로 더이상 필요없으니 지운다.
						_waitingPlayerInRooms.erase(packet._roomNum);
					}
				}
			}
			// If packet id of received packet is wrong
			else
			{
				LibLogger::cError << "MatchMother doesn't want this kind of packet. packet id : " << packetInfo->_packetId << std::endl;
			}
			
			popUsedPacket();
		}
		// stop flag activated
		// TODO: stop processing. ex) cleanup memory
		// stop all matches
		for (auto& match : _matchList)
		{
			// TODO: 일단 playerside 아무거나 넣음
			match.second->EndMatch(DEF::PlayerSide::BOTTOM);
		}
	}

	void MatchMother::StartMatch(std::pair<PlayerInfo, PlayerInfo> sessionIds, int roomNum)
	{
		auto newMatch = std::make_shared<Match>(sessionIds, _broadCaster,this,roomNum);

		_matchList.try_emplace(roomNum, newMatch);

		auto matchThread = std::thread(std::bind(&Match::Run, newMatch));
		matchThread.detach();
	}

	void MatchMother::EndMatch(int roomNum)
	{
		_matchList.erase(roomNum);
	}

}