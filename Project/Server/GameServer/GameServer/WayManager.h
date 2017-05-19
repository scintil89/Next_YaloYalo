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

		//���ڷ� �ѱ� wayIdStack�� �������ش�
		static void InitWayIdStack(std::stack<int>& wayIdStack, const DEF::Vec2& myPos, const DEF::PlayerSide mySide);

		//wayId�� wayPoint�� ��´�
		static const DEF::Vec2& GetWayPoint(int wayId, DEF::PlayerSide mySide);
		
		static const DEF::Vec2& GetWayPointSize() { return _WAYPOINT_SIZE; }

		//��ȿ�� ��ġ�� �ƴ� ������ �̵����� �� ��ȿ�� ��ġ�� �������ش�
		//true ��ȯ: ������ �߻���
		//false ��ȯ: ������ �߻����� ����
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