#pragma once

#include "Unit.h"
#include "Statement.h"

namespace LibLogic
{
	class GoblinBarrel : public Unit
	{
	public:
		GoblinBarrel(const UnitSpec& us, DEF::PlayerSide playerSide, const DEF::Vec2& position, int unitNum, Match* match, UnitManager* unitManager);
		~GoblinBarrel() = default;
		void EndSummon() override;
		void Update(float dt) override;

	private:
		void NotifyGoblinSettings();
		Statement _notifyGoblinSettings;
	};
}