#pragma once

#include "UnitState.h"
#include "define.h"

namespace LibLogic
{
	class UnitState_WalkAndSeek : public UnitState
	{
	public:
		void StartState(std::shared_ptr<Unit> unit) override;
		void RunState(std::shared_ptr<Unit> unit, float dt) override;
		void EndState(std::shared_ptr<Unit> unit) override;

	private:
		void CalculateMoveValue(std::shared_ptr<Unit> unit);
		bool IsWayPointHasUpdated(std::shared_ptr<Unit> unit);

	private:
		DEF::Vec2 _currentWayPoint;
		DEF::RECT _currentWayPointRect;
		DEF::Vec2 _wayPointRectSize;
	};
}