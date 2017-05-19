#include "Prince.h"
#include "Packets.h"
#include "Match.h"

namespace LibLogic
{
	Prince::Prince(const UnitSpec& us, DEF::PlayerSide playerSide, const DEF::Vec2& position, int unitNum, Match* match, UnitManager* unitManager) :
		Unit(us, playerSide, position, unitNum, match, unitManager)
	{
		_moveAmount = 0.0f;
		_specialAmount = 400.0f;
		_usualAttackPower = _attackPower;
		_spceialAttackPower = _attackPower * 2;
		_usualMoveSpeed = _moveSpeed;
		_specialMoveSpeed = _moveSpeed * 1.75f;
		_isSpecialState = false;
	}

	void Prince::MoveTowardsTarget(float dt)
	{
		UpdateInternalState(dt);
	
		Unit::MoveTowardsTarget(dt);
	}

	void Prince::MoveTowardsWay(float dt)
	{
		UpdateInternalState(dt);

		Unit::MoveTowardsWay(dt);
	}

	void Prince::NotifyUnitState()
	{
		using namespace Packets;

		PACKET_UNIT_SPECIAL_NTF pkt;
		pkt._fval1 = _moveSpeed;
		pkt._ival = _isSpecialState ? 1 : 0;
		pkt._unitNum = _unitNum;

		_match->SendPacketToPlayer(DEF::PlayerSide::TOP, ID_PACKET_UNIT_SPECIAL_NTF, pkt);
		_match->SendPacketToPlayer(DEF::PlayerSide::BOTTOM, ID_PACKET_UNIT_SPECIAL_NTF, pkt);
	}

	bool Prince::AttackOnce(float dt)
	{
		auto attackSuccess = Unit::AttackOnce(dt);
		
		if (attackSuccess) {
			ChangeToUsual();
			return true;
		} 
		else {
			return false;
		}
	}

	void Prince::UpdateInternalState(float dt)
	{
		if (_isSpecialState) {
			return;
		}
		
		_moveAmount += _moveSpeed * dt;
		if (_moveAmount > _specialAmount) {
			_moveAmount = 0.0f;
			ChangeToSpecial();
		}
	}

	void Prince::ChangeToSpecial()
	{
		if (_isSpecialState) {
			return;
		}

		_moveValue = _moveValue / _moveSpeed;
		_moveSpeed = _specialMoveSpeed;
		_moveValue = _moveValue * _moveSpeed;
		_attackPower = _spceialAttackPower;
		_isSpecialState = true;

		NotifyUnitState();
	}
	
	void Prince::ChangeToUsual()
	{
		if (!_isSpecialState) {
			return;
		}

		_moveValue = _moveValue / _moveSpeed;
		_moveSpeed = _usualMoveSpeed;
		_moveValue = _moveValue * _moveSpeed;
		_attackPower = _usualAttackPower;
		_isSpecialState = false;

		NotifyUnitState();
	}
}