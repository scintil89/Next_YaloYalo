#pragma once

#include <string>
#include "define.h"

using namespace std;
using namespace LibLogic;
namespace LibLogic
{
	using namespace DEF;
}

//모든 객체의 Anchor Point는 Middle입니다
namespace MapSpecs
{
	//Map
	const Vec2 MAP_SIZE = Vec2{ 700.f, 900.f };
	const float MAP_SIZE_LEFT_EDGE = -(MAP_SIZE._x / 2);
	const float MAP_SIZE_RIGHT_EDGE = MAP_SIZE._x / 2;
	const float MAP_SIZE_BOTTOM_EDGE = -(MAP_SIZE._y / 2);
	const float MAP_SIZE_TOP_EDGE = MAP_SIZE._y / 2;

	//Top Buildings
	const Vec2 TOP_KING_POS = Vec2{ 0.f, 390.f };
	const Vec2 RIGHT_TOP_BUILDING_POS = Vec2{ 250.f, 250.f };
	const Vec2 LEFT_TOP_BUILDING_POS = Vec2{ -250.f, 250.f };

	//Bottom Buildings
	const Vec2 BOTTOM_KING_POS = Vec2{ 0.f, -390.f };
	const Vec2 RIGHT_BOTTOM_BUILDING_POS = Vec2{ 250.f, -250.f };
	const Vec2 LEFT_BOTTOM_BUILDING_POS = Vec2{ -250.f, -250.f };
	
	//Bridge
	const Vec2 BRIDGE_SIZE = Vec2{ 50.f, 125.f };
	
	const Vec2 LEFT_BRIDGE_POS = Vec2{ -250.f, 0.f };
	const Vec2 LEFT_BRIDGE_TOP_POS = Vec2{ LEFT_BRIDGE_POS._x, BRIDGE_SIZE._y / 2 };
	const Vec2 LEFT_BRIDGE_BOTTOM_POS = Vec2{ LEFT_BRIDGE_POS._x, -BRIDGE_SIZE._y / 2 };
	
	const Vec2 RIGHT_BRIDGE_POS = Vec2{ 250.f, 0.f };
	const Vec2 RIGHT_BRIDGE_TOP_POS = Vec2{ RIGHT_BRIDGE_POS._x, BRIDGE_SIZE._y / 2 };
	const Vec2 RIGHT_BRIDGE_BOTTOM_POS = Vec2{ RIGHT_BRIDGE_POS._x, -BRIDGE_SIZE._y / 2 };
	
	const DEF::RECT LEFT_BRIDGE_AREA = DEF::RECT{LEFT_BRIDGE_POS._x, LEFT_BRIDGE_POS._y, BRIDGE_SIZE._x, BRIDGE_SIZE._y};
	const DEF::RECT RIGHT_BRIDGE_AREA = DEF::RECT{RIGHT_BRIDGE_POS._x, RIGHT_BRIDGE_POS._y, BRIDGE_SIZE._x, BRIDGE_SIZE._y};

	//Invalid Area(River)
	//- Prefix __ is for an internal variable used in calculation
	
	//- 높이와 y위치 값은 동일하다 (넓이와 x위치만 차이가 있다)
	const float __INVALID_AREA_HEIGHT = LEFT_BRIDGE_AREA._h;
	const float __INVALID_AREA_Y = LEFT_BRIDGE_AREA._y;

	//왼쪽 유효하지 않은 영역 = 왼쪽 다리의 왼쪽영역
	const float __INVALID_AREA_LEFT_WIDTH = LEFT_BRIDGE_AREA._xMin - MAP_SIZE_LEFT_EDGE;
	const DEF::RECT INVALID_AREA_LEFT = DEF::RECT{ MAP_SIZE_LEFT_EDGE + (__INVALID_AREA_LEFT_WIDTH / 2), __INVALID_AREA_Y, __INVALID_AREA_LEFT_WIDTH, __INVALID_AREA_HEIGHT };

	//오른쪽 유효하지 않은 영역 = 오른쪽 다리의 오른쪽 영역
	const float __INVALID_AREA_RIGHT_WIDTH = MAP_SIZE_RIGHT_EDGE - RIGHT_BRIDGE_AREA._xMax;
	const DEF::RECT INVALID_AREA_RIGHT{ MAP_SIZE_RIGHT_EDGE - (__INVALID_AREA_RIGHT_WIDTH / 2), __INVALID_AREA_Y, __INVALID_AREA_RIGHT_WIDTH, __INVALID_AREA_HEIGHT};

	//중앙 유효하지 않은 영역 = 왼쪽 다리와 오른쪽 다리의 사이 영역
	const float __INVALID_AREA_MIDDLE_WIDTH = (RIGHT_BRIDGE_AREA._xMin - LEFT_BRIDGE_AREA._xMax) / 2;
	
	const DEF::RECT INVALID_AREA_MIDDLE_LEFT = DEF::RECT{LEFT_BRIDGE_AREA._xMax + (__INVALID_AREA_MIDDLE_WIDTH / 2), __INVALID_AREA_Y, __INVALID_AREA_MIDDLE_WIDTH, __INVALID_AREA_HEIGHT};
	const DEF::RECT INVALID_AREA_MIDDLE_RIGHT = DEF::RECT{RIGHT_BRIDGE_AREA._xMin - (__INVALID_AREA_MIDDLE_WIDTH / 2), __INVALID_AREA_Y, __INVALID_AREA_MIDDLE_WIDTH, __INVALID_AREA_HEIGHT};
	
	//Misc
	const Vec2 WAYPOINT_SIZE = Vec2{ 7.5f, 7.5f };
}