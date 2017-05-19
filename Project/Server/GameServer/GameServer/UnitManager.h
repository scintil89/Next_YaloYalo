#pragma once

#include <memory>
#include <string>
#include <map>
#include <functional>
#include <list>

#include "define.h"

namespace LibLogic
{
	class Unit;
	class UnitTemplate;
	class Match;
	
	class UnitManager
	{
	public:
		UnitManager(Match* match);
		~UnitManager() = default;
		std::shared_ptr<Unit>					GetUnit(int unitNum);
		bool SummonUnit(const std::string& unitName, const DEF::Vec2& position, const DEF::PlayerSide playerSide);
		void									UpdateAllUnits(float dt);
		std::list<int>&							FindUnitsByCondition(int callerNum, std::function<bool(Unit* caller, Unit* other)> condition);
		void									KillUnit(int unitNum);

	private:
		std::shared_ptr<Unit>					CreateUnit(const std::string& unitName, const DEF::Vec2& position, const DEF::PlayerSide playerSide);
		void									NotifySummon(const std::string& unitName, int unitNum, int hp, const DEF::Vec2& position, const DEF::PlayerSide playerSide);
	
	private:
		std::map<int,std::shared_ptr<Unit>>		_unitList;
		std::list<int>							_tempUnitList;
		int										_lastUnitIndex = 0;
		Match*									_match;
		UnitTemplate*								_unitSpecs;
	};
}