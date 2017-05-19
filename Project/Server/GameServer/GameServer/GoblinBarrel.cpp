#include "GoblinBarrel.h"
#include "Match.h"
#include "Packets.h"
#include "Logger.h"
#include <functional>
#include "MapSpecs.h"

namespace LibLogic
{
	GoblinBarrel::GoblinBarrel(const UnitSpec& us, DEF::PlayerSide playerSide, const DEF::Vec2& position, int unitNum, Match* match, UnitManager* unitManager) :
		Unit(us, playerSide, position, unitNum, match, unitManager)
	{
		_notifyGoblinSettings = Statement(std::bind(&GoblinBarrel::NotifyGoblinSettings, this));
	}

	void GoblinBarrel::EndSummon()
	{
		_isMagic = false;
	}

	void GoblinBarrel::Update(float dt)
	{
		_notifyGoblinSettings.ExecuteOnce();

		Unit::Update(dt);
	}

	void GoblinBarrel::NotifyGoblinSettings()
	{
		float kingPosY = (_ownerPlayer == DEF::PlayerSide::TOP) ? MapSpecs::TOP_KING_POS._y : MapSpecs::BOTTOM_KING_POS._y;

		PACKET_UNIT_SPECIAL_NTF pkt;
		pkt._unitNum = _unitNum;
		pkt._fval1 = kingPosY;
		pkt._fval2 = _timeToSummon;

		_match->SendPacketToPlayer(DEF::PlayerSide::BOTTOM, PacketId::ID_PACKET_UNIT_SPECIAL_NTF, pkt);
		_match->SendPacketToPlayer(DEF::PlayerSide::TOP, PacketId::ID_PACKET_UNIT_SPECIAL_NTF, pkt);
	}
}