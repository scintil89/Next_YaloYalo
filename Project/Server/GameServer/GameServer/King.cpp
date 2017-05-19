#include "King.h"
#include "Match.h"
#include "Packets.h"
#include "Logger.h"

namespace LibLogic
{
	King::King(const UnitSpec& us, DEF::PlayerSide playerSide, const DEF::Vec2& position, int unitNum, Match* match, UnitManager* unitManager) :
		Unit(us, playerSide, position, unitNum, match, unitManager)
	{
	}

	void King::Update(float dt)
	{
		using namespace Packets;

		Unit::Update(dt);

		//King이 사망했을 경우에 승패처리를 한다
		if (_hp <= 0)
		{
			bool winSide = !_ownerPlayer; //상대편이 이김

			PACKET_GAME_RESULT_NTF gameResNtf;
			gameResNtf._winPlayerSide = winSide;

			_match->SendPacketToPlayer(DEF::PlayerSide::BOTTOM, PacketId::ID_PACKET_GAME_RESULT_NTF, gameResNtf);
			_match->SendPacketToPlayer(DEF::PlayerSide::TOP, PacketId::ID_PACKET_GAME_RESULT_NTF, gameResNtf);

			_match->EndMatch((DEF::PlayerSide)winSide);

			LibLogger::cDebug << "Player [" << winSide << "] win" << std::endl;
		}
	}

}

