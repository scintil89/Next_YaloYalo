#include "UnitState_Summoning.h"
#include "UnitState_WalkAndSeek.h"
#include "UnitState_Nothing.h"
#include "Unit.h"

#include "Logger.h"

namespace LibLogic 
{
	void UnitState_Summoning::StartState(std::shared_ptr<Unit> unit)
	{
		LibLogger::cDebug << "Unit [" << unit->GetUnitName() << "(" << unit->GetUnitNum() << ")] is under summoning!" << std::endl;

		_summonCompleteTime = unit->GetTimeToSummon();
	}

	void UnitState_Summoning::RunState(std::shared_ptr<Unit> unit, float dt)
	{
		_elapsedTime += dt;
		if (_elapsedTime > _summonCompleteTime)
		{
			unit->ChangeState<UnitState_WalkAndSeek>();
		}
	}

	void UnitState_Summoning::EndState(std::shared_ptr<Unit> unit)
	{
		unit->EndSummon();
	}
}