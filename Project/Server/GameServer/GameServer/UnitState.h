#pragma once

#include <memory>

namespace LibLogic
{
	class Unit;
	class UnitState
	{
	public:
		virtual void StartState(std::shared_ptr<Unit> unit) = 0;
		virtual void RunState(std::shared_ptr<Unit> unit, float dt) = 0;
		virtual void EndState(std::shared_ptr<Unit> unit) = 0;
	};
}