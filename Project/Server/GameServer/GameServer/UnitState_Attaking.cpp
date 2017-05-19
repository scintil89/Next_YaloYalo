#include "UnitState_Attaking.h"
#include "UnitState_WalkAndSeek.h"
#include "Unit.h"

#include "Logger.h"

namespace LibLogic
{
	void UnitState_Attaking::StartState(std::shared_ptr<Unit> unit)
	{
		if (unit->GetTargetObject() == nullptr)
		{
			unit->ChangeState<UnitState_WalkAndSeek>();
			return;
		}

		// 일단 한대 때린다
		unit->AttackOnce(0.0f);

		LibLogger::cDebug << "Unit [" << unit->GetUnitName() << "(" << unit->GetUnitNum() << ")] starts attacking unit ["
			<< unit->GetTargetObject()->GetUnitName() << "(" << unit->GetTargetObject()->GetUnitNum() << ")] !!" << std::endl;
	}

	void UnitState_Attaking::RunState(std::shared_ptr<Unit> unit, float dt)
	{
		auto target = unit->GetTargetObject();

		// 타겟이 죽었거나 공격범위를 벗어났으면 다시 걷는다.
		if (target == nullptr || target->GetIsDead() ||
			DEF::GetDistance(unit->GetPosition(), target->GetPosition()) > unit->GetAttackRange())
		{
			unit->ChangeState<UnitState_WalkAndSeek>();
			return;
		}

		// 시간 됐으면 때린다.
		_elapsedTimeFromLastAttack += dt;
		if (_elapsedTimeFromLastAttack >= unit->GetAttackFreq())
		{
			unit->AttackOnce(dt);
			_elapsedTimeFromLastAttack = 0.f;
		}
	}

	void UnitState_Attaking::EndState(std::shared_ptr<Unit> unit)
	{
		unit->EndAttack();
	}
}