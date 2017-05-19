#pragma once

#include "UnitState.h"

namespace LibLogic
{
	class UnitState_Nothing : public UnitState
	{
	public:
		void StartState(std::shared_ptr<Unit> unit) override;
		void RunState(std::shared_ptr<Unit> unit, float dt) override;
		void EndState(std::shared_ptr<Unit> unit) override;
	};
}

