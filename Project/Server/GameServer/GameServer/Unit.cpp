#include "Unit.h"
#include "Match.h"
#include "Packets.h"
#include "Logger.h"
#include "WayManager.h"

namespace LibLogic
{
	Unit::Unit(const UnitSpec& us, DEF::PlayerSide playerSide, const DEF::Vec2& position, int unitNum, Match* match, UnitManager* unitManager) :
		_ownerPlayer(playerSide),
		_position(position),
		_unitNum(unitNum),
		_match(match),
		_unitManager(unitManager),
		_state(nullptr),
		_isDead(false),
		_targetNum(-1) 
	{
		_unitName = us.unitName;
		_timeToSummon = us.summonTime;
		_moveSpeed = us.moveSpeed;
		_sightRange = us.sightRange;
		_attackPower = us.attackPower;
		_attackRange = us.attackRange;
		_attackFreq = us.attackFreq;
		_attackDelay = us.attackDelay;
		_hp = us.hp;
		_isMagic = us.isMagic;
	}

	void Unit::Update(float dt)
	{
		// 상태 갱신
		_state->RunState(shared_from_this(), dt);

		// 데미지 처리
		ProcessDamage(dt);

		// 사망 처리
		ProcessDead();
	}

	void Unit::NotifyMoveToTarget()
	{
		auto target = GetTargetObject();
		if (target == nullptr)
		{
			return;
		}

		Packets::PACKET_UNIT_MOVE_TO_TARGET_NTF packet;
		packet._currentPosX = _position._x;
		packet._currentPosY = _position._y;
		packet._speed = _moveSpeed;
		packet._unitNum = _unitNum;
		packet._targetNum = _targetNum;

		_match->SendPacketToPlayer(DEF::PlayerSide::BOTTOM, Packets::PacketId::ID_PACKET_UNIT_MOVE_TO_TARGET_NTF, packet);
		_match->SendPacketToPlayer(DEF::PlayerSide::TOP, Packets::PacketId::ID_PACKET_UNIT_MOVE_TO_TARGET_NTF, packet);
	}

	void Unit::NotifyMoveToWaypoint(const DEF::Vec2& wayPoint)
	{
		Packets::PACKET_UNIT_MOVE_TO_WAYPOINT_NTF packet;
		packet._unitNum = _unitNum;
		packet._moveValueX = _moveValue._x;
		packet._moveValueY = _moveValue._y;
		packet._wayPointX = wayPoint._x;
		packet._wayPointY = wayPoint._y;

		_match->SendPacketToPlayer(DEF::PlayerSide::BOTTOM, Packets::PacketId::ID_PACKET_UNIT_MOVE_TO_WAYPOINT_NTF, packet);
		_match->SendPacketToPlayer(DEF::PlayerSide::TOP, Packets::PacketId::ID_PACKET_UNIT_MOVE_TO_WAYPOINT_NTF, packet);
	}

	void Unit::NotifyUnitState()
	{
		//용도: 특별상태가 있는 유닛은 이 함수를 구현한다
	}

	bool Unit::AttackOnce(float dt)
	{
		auto target = GetTargetObject();
		if (target == nullptr || 
			target->GetIsDead() == true)
		{
			return false;
		}

		target->ReserveToHit(_attackPower, _attackDelay);

		Packets::PACKET_UNIT_ATTACK_NTF packet;
		packet._unitNum = _unitNum;
		packet._targetNum = _targetNum;
		packet._attackDelay = _attackDelay;

		_match->SendPacketToPlayer(DEF::PlayerSide::BOTTOM, Packets::PacketId::ID_PACKET_UNIT_ATTACK_NTF, packet);
		_match->SendPacketToPlayer(DEF::PlayerSide::TOP, Packets::PacketId::ID_PACKET_UNIT_ATTACK_NTF, packet);

		return true;
	}

	void Unit::MoveTowardsTarget(float dt)
	{
		auto target = GetTargetObject();
		if (target == nullptr)
		{
			return;
		}

		auto targetPos = target->GetPosition();
		auto deltaVec = targetPos - _position;
		auto distance = DEF::GetDistance(targetPos, _position);
		auto unitVec = deltaVec / distance;
		auto moveDelta = unitVec * _moveSpeed;

		_position._x += moveDelta._x * dt;
		_position._y += moveDelta._y * dt;
	}

	void Unit::MoveTowardsWay(float dt)
	{
		_position._x += _moveValue._x * dt;
		_position._y += _moveValue._y * dt;
	}

	// 시야에 들어온 적 유닛 중 하나를 타겟팅한다.
	// 마땅한 타겟이 없으면 -1 반환
	// unit클래스의 기본 설정은 가장 가까운 것을 반환
	int Unit::ScanTargetNum()
	{
		auto candidates = GetEnemyUnitsUnderSight();

		// 시야에 들어온 적 유닛 없음
		if (candidates.size() == 0)
		{
			return -1;
		}

		int scannedTargetNum = -1;
		auto minDist = FLT_MAX;

		// 후보 중 가장 가까운 것을 찾는다.
		for (auto& targetNum : candidates)
		{
			auto target = _unitManager->GetUnit(targetNum);
			auto dist = DEF::GetDistance(target->GetPosition(), _position);
			if (minDist > dist)
			{
				minDist = dist;
				scannedTargetNum = targetNum;
			}
		}

		return scannedTargetNum;
	}

	std::list<int> Unit::GetEnemyUnitsUnderSight()
	{
		// 시야 내에 들어온 적유닛을 찾는다
		auto compareFunc = [](Unit* me, Unit* other)
		{
			if (other->GetIsMagic() == true)
				return false;

			if (me->GetOwnerPlayer() == other->GetOwnerPlayer())
				return false;

			if (DEF::GetDistance(me->GetPosition(), other->GetPosition()) > me->GetSightRange())
				return false;

			return true;
		};

		return _unitManager->FindUnitsByCondition(_unitNum, compareFunc);
	}

	void Unit::ReserveToHit(int damage, float delay)
	{
		_damageList.push_back({ delay,damage });
	}

	void Unit::ProcessDamage(float dt)
	{
		// 데미지 처리
		for (std::pair<float, int>& damage : _damageList)
		{
			damage.first -= dt;
			if (damage.first <= 0.f)
			{
				_hp -= damage.second;

				PACKET_UNIT_HP_CHANGED packet;
				packet._hp = _hp;
				packet._unitNum = _unitNum;

				_match->SendPacketToPlayer(DEF::PlayerSide::TOP, PacketId::ID_PACKET_UNIT_HP_CHANGED, packet);
				_match->SendPacketToPlayer(DEF::PlayerSide::BOTTOM, PacketId::ID_PACKET_UNIT_HP_CHANGED, packet);
			}
		}

		// 데미지 후처리
		_damageList.remove_if([](auto& key) { return key.first <= 0.f ? true : false; });
	}

	void Unit::ProcessDead()
	{
		if (_hp <= 0)
		{
			_isDead = true;

			PACKET_UNIT_DIE_NTF packet;
			packet._unitNum = _unitNum;

			_match->SendPacketToPlayer(DEF::PlayerSide::BOTTOM, PacketId::ID_PACKET_UNIT_DIE_NTF, packet);
			_match->SendPacketToPlayer(DEF::PlayerSide::TOP, PacketId::ID_PACKET_UNIT_DIE_NTF, packet);

			LibLogger::cDebug << "Unit [" << _unitName << "(" << _unitNum << ")] is dead" << std::endl;
		}
	}

	int Unit::GetWayId()
	{
		if (_wayIdStack.size() != 0)
		{
			int wayId = _wayIdStack.top();
			_wayIdStack.pop();
			return wayId;
		}
		return -1;
	}

	Unit* Unit::GetTargetObject()
	{
		return _unitManager->GetUnit(_targetNum).get();
	}
}