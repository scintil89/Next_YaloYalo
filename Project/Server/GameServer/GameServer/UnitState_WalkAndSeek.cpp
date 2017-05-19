#include "UnitState_WalkAndSeek.h"
#include "UnitState_Approach.h"
#include "UnitState_Nothing.h"
#include "Unit.h"

#include "WayManager.h"
#include "Logger.h"

namespace LibLogic
{
	void UnitState_WalkAndSeek::StartState(std::shared_ptr<Unit> unit)
	{
		//Unit의 WayPoint를 초기화한다
		WayManager::InitWayIdStack(unit->GetWayIdStack(), unit->GetPosition(), unit->GetOwnerPlayer());

		//WalkAndSeek State용 변수들을 초기화한다
		_currentWayPoint = WayManager::GetWayPoint(unit->GetWayId(), unit->GetOwnerPlayer());
		_wayPointRectSize = WayManager::GetWayPointSize();
		_currentWayPointRect = DEF::RECT{ _currentWayPoint._x, _currentWayPoint._y, _wayPointRectSize._x, _wayPointRectSize._y };

		//Unit이 어떻게 움직일지 계산한다
		auto myPos = unit->GetPosition();
		auto lookVector = _currentWayPoint - myPos;
		auto distance = DEF::GetDistance(_currentWayPoint, myPos);
		auto normalizedLookVector = lookVector / distance;
		auto moveVal = normalizedLookVector * unit->GetmoveSpeed();

		unit->SetMoveValue(moveVal);

		//통지
		unit->NotifyMoveToWaypoint(_currentWayPoint);

		LibLogger::cDebug << "Unit [" << unit->GetUnitName() << 
			"(" << unit->GetUnitNum() << ")] starts seeking proper target!" << std::endl;
	}

	void UnitState_WalkAndSeek::RunState(std::shared_ptr<Unit> unit, float dt)
	{
		//공격타깃을 찾으면 접근 상태로 전환
		auto properTargetNum = unit->ScanTargetNum();
		if (properTargetNum != -1)
		{
			unit->SetTargetNum(properTargetNum);
			unit->ChangeState<UnitState_Approach>();
			return;
		}

		//웨이포인트 갱신해야하면 갱신
		if (IsWayPointHasUpdated(unit))
		{
			//갱신된 웨이포인트를 반영한 움직임을 계산한다
			CalculateMoveValue(unit);

			//통지는 모든 변화가 계산된 뒤 한다
			unit->NotifyMoveToWaypoint(_currentWayPoint);
		}

		//웨이포인트로 이동
		unit->MoveTowardsWay(dt);
	}

	void UnitState_WalkAndSeek::CalculateMoveValue(std::shared_ptr<Unit> unit)
	{
		auto myPos = unit->GetPosition();
		auto lookVector = _currentWayPoint - myPos;
		auto distance = DEF::GetDistance(_currentWayPoint, myPos);
		auto normalizedLookVector = lookVector / distance;
		auto moveVal = normalizedLookVector * unit->GetmoveSpeed();
		
		unit->SetMoveValue(moveVal);
	}

	bool UnitState_WalkAndSeek::IsWayPointHasUpdated(std::shared_ptr<Unit> unit)
	{
		//웨이 포인트에 도달했으면
		if (_currentWayPointRect.Contains(unit->GetPosition()))
		{
			//다음 웨이포인트로 갱신
			auto wayId = unit->GetWayId();

			//더 이상 갈 곳이 없으면(=킹 타워까지 점령했으면)
			if (wayId == -1)
			{
				//멈춘다 
				unit->ChangeState<UnitState_Nothing>();

				//TODO: 멈추기 패킷 보내기

				return false;
			}

			_currentWayPoint = WayManager::GetWayPoint(wayId, unit->GetOwnerPlayer());
			_currentWayPointRect = DEF::RECT{ _currentWayPoint._x, _currentWayPoint._y, _wayPointRectSize._x, _wayPointRectSize._y };
			
			return true;
		}

		return false;
	}

	void UnitState_WalkAndSeek::EndState(std::shared_ptr<Unit> unit)
	{
	}
}