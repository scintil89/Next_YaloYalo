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

		//Ÿ���� ��ȿ���� ������ ���ο� Ÿ���� ã�´�
		if (target == nullptr)
		{
			unit->ChangeState<UnitState_WalkAndSeek>();
			return;
		}
		//���� ���� ���� �������� �����Ѵ�
		if (unit->GetAttackRange() > DEF::GetDistance(target->GetPosition(), unit->GetPosition()))
		{
			unit->ChangeState<UnitState_Attaking>();
			return;
		}

		//��� �����Ѵ�
		unit->MoveTowardsTarget(dt);
	}

	void UnitState_Approach::EndState(std::shared_ptr<Unit> unit)
	{
	}
}