#pragma once

#include "Unit.h"
#include "Statement.h"
#include "Clock.h"

namespace LibLogic
{
	class Sparky : public Unit
	{
	public:
		Sparky(const UnitSpec& us, DEF::PlayerSide playerSide, const DEF::Vec2& position, int unitNum, Match* match, UnitManager* unitManager);
		~Sparky() = default;
		void Update(float dt) override;
		bool AttackOnce(float dt) override;
		void EndAttack() override;

	private:
		void AttackUnitsAroundTarget();
		std::list<int> FindUnitsAroundTarget();
		void NotifyChargeTime();
		void NotifyShootDelay();
		void NotifyWaitForCharge();

	private:
		float _chargeCompleteTime;
		float _splashRange;
		Statement _notifyChargeTime;
		Statement _notifyWaitForCharge;
		Statement _notifyShootDelay;
		Clock _chargeClock;
		Clock _shootDelayClock;
	};
}