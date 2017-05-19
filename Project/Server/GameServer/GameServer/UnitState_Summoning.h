#pragma once

#include "UnitState.h"

namespace LibLogic
{
	class UnitState_Summoning : public UnitState
	{
	public:
		void StartState(std::shared_ptr<Unit> unit) override;
		void RunState(std::shared_ptr<Unit> unit, float dt) override;
		void EndState(std::shared_ptr<Unit> unit) override;
	private:
		float _elapsedTime = 0.f;
		float _summonCompleteTime = 0.f;
	};
}