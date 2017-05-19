#include "WayManager.h"
#include "Unit.h"
#include "MapSpecs.h"

namespace LibLogic
{
	DEF::Vec2 WayManager::_wayPoints[WAY_POINT_NUM]
	{
		MapSpecs::BOTTOM_KING_POS,				//BOTTOM_KING,
		MapSpecs::TOP_KING_POS,				//TOP_KING,
		MapSpecs::RIGHT_BOTTOM_BUILDING_POS,	//RIGHT_BOTTOM_BUILDING,
		MapSpecs::RIGHT_BRIDGE_POS,			//RIGHT_BRIDGE,
		MapSpecs::RIGHT_BRIDGE_TOP_POS,		//RIGHT_BRIDGE_TOP,
		MapSpecs::RIGHT_BRIDGE_BOTTOM_POS,		//RIGHT_BRIDGE_BOTTOM,
		MapSpecs::RIGHT_TOP_BUILDING_POS,		//RIGHT_TOP_BUILDING,
		MapSpecs::LEFT_BOTTOM_BUILDING_POS,	//LEFT_BOTTOM_BUILDING,
		MapSpecs::LEFT_BRIDGE_POS,				//LEFT_BRIDGE,
		MapSpecs::LEFT_BRIDGE_TOP_POS,			//LEFT_BRIDGE_TOP,
		MapSpecs::LEFT_BRIDGE_BOTTOM_POS,		//LEFT_BRIDGE_BOTTOM,
		MapSpecs::LEFT_TOP_BUILDING_POS		//LEFT_TOP_BUILDING,
	};

	DEF::Vec2 WayManager::_WAYPOINT_SIZE = MapSpecs::WAYPOINT_SIZE;

	DEF::RECT WayManager::_bridgeAreas[BRIDGE_AREA_NUM]
	{
		MapSpecs::LEFT_BRIDGE_AREA,
		MapSpecs::RIGHT_BRIDGE_AREA
	};

	//비용: if문 6번
	//윗편이냐 아랫편이냐						  --- 1
	//오른쪽이냐 왼쪽이냐						  --- 1
	//1번섹터? 2번섹터? 3번섹텨? 4번섹터?		  --- 4
	void WayManager::InitWayIdStack(std::stack<int>& wayIdStack, const DEF::Vec2& myPos, const DEF::PlayerSide mySide)
	{
		//스택 비우기
		while (wayIdStack.empty() == false)
		{
			wayIdStack.pop();
		}

		//윗편인 경우
		if (mySide == DEF::PlayerSide::TOP)
		{
			//최종 목적지는 왕 타워
			wayIdStack.push(WayPoint::BOTTOM_KING);

			//왼편에 있을 경우
			if (myPos._x < 0)
			{
				if (myPos._y > MapSpecs::LEFT_BOTTOM_BUILDING_POS._y)
				{
					wayIdStack.push(WayPoint::LEFT_BOTTOM_BUILDING);
				}
				if (myPos._y > MapSpecs::LEFT_BRIDGE_BOTTOM_POS._y)
				{
					wayIdStack.push(WayPoint::LEFT_BRIDGE_BOTTOM);
				}
				if (myPos._y > MapSpecs::LEFT_BRIDGE_TOP_POS._y)
				{
					wayIdStack.push(WayPoint::LEFT_BRIDGE_TOP);
				}
				if (myPos._y > MapSpecs::LEFT_TOP_BUILDING_POS._y)
				{
					wayIdStack.push(WayPoint::LEFT_TOP_BUILDING);
				}
			}
			//오른편에 있을 경우
			else
			{
				if (myPos._y > MapSpecs::RIGHT_BOTTOM_BUILDING_POS._y)
				{
					wayIdStack.push(WayPoint::RIGHT_BOTTOM_BUILDING);
				}
				if (myPos._y > MapSpecs::RIGHT_BRIDGE_BOTTOM_POS._y)
				{
					wayIdStack.push(WayPoint::RIGHT_BRIDGE_BOTTOM);
				}
				if (myPos._y > MapSpecs::RIGHT_BRIDGE_TOP_POS._y)
				{
					wayIdStack.push(WayPoint::RIGHT_BRIDGE_TOP);
				}
				if (myPos._y > MapSpecs::RIGHT_TOP_BUILDING_POS._y)
				{
					wayIdStack.push(WayPoint::RIGHT_TOP_BUILDING);
				}
			}
		}
		//아랫편인 경우
		else
		{
			//최종 목적지는 왕타워
			wayIdStack.push(WayPoint::TOP_KING);

			//왼편인 경우
			if (myPos._x < 0)
			{
				if (myPos._y < MapSpecs::LEFT_TOP_BUILDING_POS._y)
				{
					wayIdStack.push(WayPoint::LEFT_TOP_BUILDING);
				}
				if (myPos._y < MapSpecs::LEFT_BRIDGE_TOP_POS._y)
				{
					wayIdStack.push(WayPoint::LEFT_BRIDGE_TOP);
				}
				if (myPos._y < MapSpecs::LEFT_BRIDGE_BOTTOM_POS._y)
				{
					wayIdStack.push(WayPoint::LEFT_BRIDGE_BOTTOM);
				}
				if (myPos._y < MapSpecs::LEFT_BOTTOM_BUILDING_POS._y)
				{
					wayIdStack.push(WayPoint::LEFT_BOTTOM_BUILDING);
				}
			}
			//오른편인 경우
			else
			{
				if (myPos._y < MapSpecs::RIGHT_TOP_BUILDING_POS._y)
				{
					wayIdStack.push(WayPoint::RIGHT_TOP_BUILDING);
				}
				if (myPos._y < MapSpecs::RIGHT_BRIDGE_TOP_POS._y)
				{
					wayIdStack.push(WayPoint::RIGHT_BRIDGE_TOP);
				}
				if (myPos._y < MapSpecs::RIGHT_BRIDGE_BOTTOM_POS._y)
				{
					wayIdStack.push(WayPoint::RIGHT_BRIDGE_BOTTOM);
				}
				if (myPos._y < MapSpecs::RIGHT_BOTTOM_BUILDING_POS._y)
				{
					wayIdStack.push(WayPoint::RIGHT_BOTTOM_BUILDING);
				}
			}
		}
	}

	const DEF::Vec2& WayManager::GetWayPoint(int wayId, DEF::PlayerSide mySide)
	{
		if (wayId < 0 || wayId > WAY_POINT_NUM)
		{
			return _wayPoints[0];
		}

		return _wayPoints[wayId];
	}

	bool WayManager::TryValidateMyPosition(std::shared_ptr<Unit> unit)
	{
		auto myPos = unit->GetPosition();
		//오른쪽으로 옮긴다
		if (MapSpecs::INVALID_AREA_LEFT.Contains(myPos))
		{
			return true;
		}
		//왼쪽으로 옮긴다
		if (MapSpecs::INVALID_AREA_MIDDLE_LEFT.Contains(myPos))
		{
			return true;
		}
		//
		if (MapSpecs::INVALID_AREA_MIDDLE_RIGHT.Contains(myPos))
		{
			return true;
		}
		if (MapSpecs::INVALID_AREA_RIGHT.Contains(myPos))
		{
			return true;
		}

		return false;
	}
}
