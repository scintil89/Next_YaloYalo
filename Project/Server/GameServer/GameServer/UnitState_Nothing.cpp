#include "UnitState_Nothing.h"
#include "Unit.h"

namespace LibLogic
{
	void UnitState_Nothing::StartState(std::shared_ptr<Unit> unit)
	{
	}

	void UnitState_Nothing::RunState(std::shared_ptr<Unit> unit, float dt)
	{
	}

	void UnitState_Nothing::EndState(std::shared_ptr<Unit> unit)
	{
	}
}

