#pragma once

#include <stack>
#include "define.h"

namespace LibLogic
{
	class Unit;
	class WayManager
	{
	public:
		WayManager() = delete;
		~WayManager() = delete;

		//인자로 넘긴 wayIdStack을 세팅해준다
		static void InitWayIdStack(std::stack<int>& wayIdStack, const DEF::Vec2& myPos, const DEF::PlayerSide mySide);

		//wayId로 wayPoint를 얻는다
		static const DEF::Vec2& GetWayPoint(int wayId, DEF::PlayerSide mySide);
		
		static const DEF::Vec2& GetWayPointSize() { return _WAYPOINT_SIZE; }

		//유효한 위치가 아닌 곳으로 이동했을 시 유효한 위치로 조정해준다
		//true 반환: 조정이 발생함
		//false 반환: 조정이 발생하지 않음
		static bool TryValidateMyPosition(std::shared_ptr<Unit> unit);

	private:
		enum WayPoint : int
		{
			BOTTOM_KING,
			TOP_KING,
			RIGHT_BOTTOM_BUILDING,
			RIGHT_BRIDGE,
			RIGHT_BRIDGE_TOP,
			RIGHT_BRIDGE_BOTTOM,
			RIGHT_TOP_BUILDING,
			LEFT_BOTTOM_BUILDING,
			LEFT_BRIDGE,
			LEFT_BRIDGE_TOP,
			LEFT_BRIDGE_BOTTOM,
			LEFT_TOP_BUILDING,
			WAY_POINT_NUM
		};

		enum BridgeArea : int
		{
			LEFT_BRIDGE_RECT,
			RIGHT_BRIDGE_RECT,
			BRIDGE_AREA_NUM
		};

		static DEF::Vec2 _wayPoints[WAY_POINT_NUM];
		static DEF::RECT _bridgeAreas[BRIDGE_AREA_NUM];
		static DEF::Vec2 _WAYPOINT_SIZE;
	};
}