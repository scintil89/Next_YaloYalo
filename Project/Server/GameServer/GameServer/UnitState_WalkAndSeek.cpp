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
		//Unit�� WayPoint�� �ʱ�ȭ�Ѵ�
		WayManager::InitWayIdStack(unit->GetWayIdStack(), unit->GetPosition(), unit->GetOwnerPlayer());

		//WalkAndSeek State�� �������� �ʱ�ȭ�Ѵ�
		_currentWayPoint = WayManager::GetWayPoint(unit->GetWayId(), unit->GetOwnerPlayer());
		_wayPointRectSize = WayManager::GetWayPointSize();
		_currentWayPointRect = DEF::RECT{ _currentWayPoint._x, _currentWayPoint._y, _wayPointRectSize._x, _wayPointRectSize._y };

		//Unit�� ��� �������� ����Ѵ�
		auto myPos = unit->GetPosition();
		auto lookVector = _currentWayPoint - myPos;
		auto distance = DEF::GetDistance(_currentWayPoint, myPos);
		auto normalizedLookVector = lookVector / distance;
		auto moveVal = normalizedLookVector * unit->GetmoveSpeed();

		unit->SetMoveValue(moveVal);

		//����
		unit->NotifyMoveToWaypoint(_currentWayPoint);

		LibLogger::cDebug << "Unit [" << unit->GetUnitName() << 
			"(" << unit->GetUnitNum() << ")] starts seeking proper target!" << std::endl;
	}

	void UnitState_WalkAndSeek::RunState(std::shared_ptr<Unit> unit, float dt)
	{
		//����Ÿ���� ã���� ���� ���·� ��ȯ
		auto properTargetNum = unit->ScanTargetNum();
		if (properTargetNum != -1)
		{
			unit->SetTargetNum(properTargetNum);
			unit->ChangeState<UnitState_Approach>();
			return;
		}

		//��������Ʈ �����ؾ��ϸ� ����
		if (IsWayPointHasUpdated(unit))
		{
			//���ŵ� ��������Ʈ�� �ݿ��� �������� ����Ѵ�
			CalculateMoveValue(unit);

			//������ ��� ��ȭ�� ���� �� �Ѵ�
			unit->NotifyMoveToWaypoint(_currentWayPoint);
		}

		//��������Ʈ�� �̵�
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
		//���� ����Ʈ�� ����������
		if (_currentWayPointRect.Contains(unit->GetPosition()))
		{
			//���� ��������Ʈ�� ����
			auto wayId = unit->GetWayId();

			//�� �̻� �� ���� ������(=ŷ Ÿ������ ����������)
			if (wayId == -1)
			{
				//����� 
				unit->ChangeState<UnitState_Nothing>();

				//TODO: ���߱� ��Ŷ ������

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