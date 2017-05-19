#pragma once

#include "Unit.h"

namespace LibLogic
{
	class King : public Unit
	{
	public:
		King(const UnitSpec& us, DEF::PlayerSide playerSide, const DEF::Vec2& position, int unitNum, Match* match, UnitManager* unitManager);
		~King() = default;
		void Update(float dt) override;
	};
}

