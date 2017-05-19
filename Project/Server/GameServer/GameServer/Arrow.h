#pragma once

#include "Unit.h"
#include <list>

namespace LibLogic
{
	class Arrow : public Unit
	{
	public:
		Arrow(const UnitSpec& us, DEF::PlayerSide playerSide, const DEF::Vec2& position, int unitNum, Match* match, UnitManager* unitManager);
		~Arrow() = default;

		void Update(float dt) override;

	private:
		void AttackUnitsWithinRange(float dt);
		void SetTargetNumsUnderAttackRange();
		void ProcessDead(float dt);

	private:
		std::list<int> _targetNums;
		float _arrowDieTime;
		bool _isUsed;
	};
}
