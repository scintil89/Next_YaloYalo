#pragma once

#include "define.h"

#include "UnitState.h"

namespace LibLogic
{
	class UnitState_Approach : public UnitState
	{
	public:
		void StartState(std::shared_ptr<Unit> unit) override;
		void RunState(std::shared_ptr<Unit> unit, float dt) override;
		void EndState(std::shared_ptr<Unit> unit) override;
	};
}