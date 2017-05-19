#include "Arrow.h"

namespace LibLogic
{
	Arrow::Arrow(const UnitSpec& us, DEF::PlayerSide playerSide, const DEF::Vec2& position, int unitNum, Match* match, UnitManager* unitManager) :
		Unit(us, playerSide, position, unitNum, match, unitManager)
	{
		_arrowDieTime = (float)_hp;
		_isUsed = false;
	}

	void Arrow::Update(float dt)
	{
		if (_isUsed == false)
		{
			AttackUnitsWithinRange(dt);
			_isUsed = true;
		}
		
		ProcessDead(dt);
	}

	void Arrow::AttackUnitsWithinRange(float dt)
	{
		SetTargetNumsUnderAttackRange();

		for (const int& curTargetNum : _targetNums)
		{
			_targetNum = curTargetNum;
			AttackOnce(dt);
		}
	}

	void Arrow::SetTargetNumsUnderAttackRange()
	{
		// 공격범위 내에 들어온 적유닛을 찾는다
		auto condition = [](Unit* me, Unit* other)
		{
			if (other->GetIsMagic() == true)
				return false;

			if (me->GetOwnerPlayer() == other->GetOwnerPlayer())
				return false;

			if (me->GetAttackRange() < DEF::GetDistance(other->GetPosition(), me->GetPosition()))
				return false;

			return true;
		};

		_targetNums.clear();
		_targetNums = _unitManager->FindUnitsByCondition(_unitNum, condition);
	}

	
	void Arrow::ProcessDead(float dt)
	{
		_arrowDieTime -= dt;
		if (_arrowDieTime <= 0.0f)
		{
			_hp = -1;
			Unit::ProcessDead();
		}
	}
}
