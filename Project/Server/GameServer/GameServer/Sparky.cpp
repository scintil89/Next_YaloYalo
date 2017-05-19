#include "Sparky.h"
#include "Packets.h"
#include "Match.h"
#include "Logger.h"

namespace LibLogic
{
	Sparky::Sparky(const UnitSpec& us, DEF::PlayerSide playerSide, const DEF::Vec2& position, int unitNum, Match* match, UnitManager* unitManager) :
		Unit(us, playerSide, position, unitNum, match, unitManager)
	{
		_chargeCompleteTime = 8.0f;
		_splashRange = 40.0f;

		_notifyChargeTime = Statement(std::bind(&Sparky::NotifyChargeTime, this));
		_notifyWaitForCharge = Statement(std::bind(&Sparky::NotifyWaitForCharge, this), true);
		_notifyShootDelay = Statement(std::bind(&Sparky::NotifyShootDelay, this), true);
		_shootDelayClock = Clock(_attackDelay);
		_chargeClock = Clock(_chargeCompleteTime);
	}

	void Sparky::Update(float dt)
	{
		_notifyChargeTime.ExecuteOnce();

		_chargeClock.Tick(dt);

		Unit::Update(dt);
	}

	bool Sparky::AttackOnce(float dt)
	{
		//충전 대기
		_notifyWaitForCharge.ExecuteIfTriggered();
		if (!_chargeClock.IsComplete()) {
			return false;
		}

		//공격딜레이 대기
		_notifyShootDelay.ExecuteIfTriggered();
		_shootDelayClock.Tick(dt);
		if (!_shootDelayClock.IsComplete()) {
			return false;
		}

		//공격
		AttackUnitsAroundTarget();

		//초기화
		_notifyWaitForCharge.Trigger();
		_notifyShootDelay.Trigger();
		_shootDelayClock.Reset();
		_chargeClock.Reset();

		return true;
	}

	void Sparky::EndAttack()
	{
		_notifyWaitForCharge.Trigger();
		_notifyShootDelay.Trigger();
		_shootDelayClock.Reset();
	}

	void Sparky::AttackUnitsAroundTarget()
	{
		auto targetNums = FindUnitsAroundTarget();

		for (int targetNum : targetNums)
		{
			auto target = _unitManager->GetUnit(targetNum);
			if (target == nullptr || target->GetIsDead() == true) {
				continue;
			}

			target->ReserveToHit(_attackPower, 0.0f);
		}
	}

	std::list<int> Sparky::FindUnitsAroundTarget()
	{
		// 타겟 주변의 적유닛을 찾는다
		const float SPLASH_RANGE = _splashRange;
		auto condition = [SPLASH_RANGE](Unit* target, Unit* other)
		{
			if (other->GetIsMagic() == true)
				return false;

			if (target->GetOwnerPlayer() != other->GetOwnerPlayer())
				return false;

			if (DEF::GetDistance(other->GetPosition(), target->GetPosition()) > SPLASH_RANGE)
				return false;

			return true;
		};
		
		return std::move(_unitManager->FindUnitsByCondition(_targetNum, condition));
	}

	void Sparky::NotifyChargeTime()
	{
		PACKET_UNIT_SPECIAL_NTF pkt;
		pkt._unitNum = _unitNum;
		pkt._fval1 = _chargeCompleteTime; 
		pkt._fval2 = _splashRange; 
		pkt._ival = 0;

		_match->SendPacketToPlayer(DEF::PlayerSide::BOTTOM, ID_PACKET_UNIT_SPECIAL_NTF, pkt);
		_match->SendPacketToPlayer(DEF::PlayerSide::TOP, ID_PACKET_UNIT_SPECIAL_NTF, pkt);
	}

	void Sparky::NotifyWaitForCharge()
	{
		PACKET_UNIT_SPECIAL_NTF pkt;
		pkt._unitNum = _unitNum;
		pkt._ival = 1;

		_match->SendPacketToPlayer(DEF::PlayerSide::BOTTOM, ID_PACKET_UNIT_SPECIAL_NTF, pkt);
		_match->SendPacketToPlayer(DEF::PlayerSide::TOP, ID_PACKET_UNIT_SPECIAL_NTF, pkt);
	}

	void Sparky::NotifyShootDelay()
	{
		Packets::PACKET_UNIT_ATTACK_NTF packet;
		packet._unitNum = _unitNum;
		packet._targetNum = _targetNum;
		packet._attackDelay = _attackDelay;

		_match->SendPacketToPlayer(DEF::PlayerSide::BOTTOM, Packets::PacketId::ID_PACKET_UNIT_ATTACK_NTF, packet);
		_match->SendPacketToPlayer(DEF::PlayerSide::TOP, Packets::PacketId::ID_PACKET_UNIT_ATTACK_NTF, packet);
	}
}