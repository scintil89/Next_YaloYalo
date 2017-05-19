#pragma once

#include "Unit.h"

namespace LibLogic
{
	class Prince : public Unit
	{
	public:
		Prince(const UnitSpec& us, DEF::PlayerSide playerSide, const DEF::Vec2& position, int unitNum, Match* match, UnitManager* unitManager);
		~Prince() = default;
		void MoveTowardsTarget(float dt) override;
		void MoveTowardsWay(float dt) override;
		void NotifyUnitState() override;
		bool AttackOnce(float dt) override;

	private:
		void UpdateInternalState(float dt);
		void ChangeToSpecial();
		void ChangeToUsual();

	private:
		float _moveAmount; //accumulation of move amounts
		float _specialAmount; //moveAmount > specialAmount ? Change to SpecialState
		float _usualMoveSpeed;
		int _usualAttackPower;
		float _specialMoveSpeed;
		int _spceialAttackPower;
		bool _isSpecialState;
	};
}
