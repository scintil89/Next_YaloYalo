#include "UnitState_Approach.h"
#include "UnitState_WalkAndSeek.h"
#include "UnitState_Attaking.h"
#include "Unit.h"

#include "Logger.h"

namespace LibLogic
{
	void UnitState_Approach::StartState(std::shared_ptr<Unit> unit)
	{
		unit->NotifyMoveToTarget();

		LibLogger::cDebug << "Unit [" << unit->GetUnitName() << "(" << unit->GetUnitNum() << ")] is approaching to unit ["
			<< unit->GetTargetObject()->GetUnitName() << "(" << unit->GetTargetObject()->GetUnitNum() << ")] !!" << std::endl;
	}

	void UnitState_Approach::RunState(std::shared_ptr<Unit> unit, float dt)
	{
		auto target = unit->GetTargetObject();

		//타겟이 유효하지 않으면 새로운 타겟을 찾는다
		if (target == nullptr)
		{
			unit->ChangeState<UnitState_WalkAndSeek>();
			return;
		}
		//공격 범위 내에 들어왔으면 공격한다
		if (unit->GetAttackRange() > DEF::GetDistance(target->GetPosition(), unit->GetPosition()))
		{
			unit->ChangeState<UnitState_Attaking>();
			return;
		}

		//계속 접근한다
		unit->MoveTowardsTarget(dt);
	}

	void UnitState_Approach::EndState(std::shared_ptr<Unit> unit)
	{
	}
}