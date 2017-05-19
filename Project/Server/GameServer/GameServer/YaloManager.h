#pragma once
#include <string>
#include <map>
#include "YaloTable.h"
#include "Player.h"
#include "Define.h"

namespace LibLogic
{
	class Match;

	class YaloManager
	{
	public:
		YaloManager(Match* match, std::map<DEF::PlayerSide, Player>& players);
		~YaloManager() = default;

		int GetCost(const std::string& unitName);
		void Update(float dt);
		void StartUpdate() { _updateFlag = true; }
		void UseYalo(DEF::PlayerSide playerSide, int usedYalo);
		auto GetStartYaloNumber() { return _yaloTable->startYaloNumber; }
		auto GetMaxYaloNumber() { return _yaloTable->maxYaloNumber; }
		auto GetYaloChargeTime() { return _yaloTable->yaloChargeTime; }

	private:
		void _Update(float dt, DEF::PlayerSide playerSide);

	private:
		YaloTable* _yaloTable;
		Match* _match;
		std::map<DEF::PlayerSide, Player>& _players;
		
		float _elapsedTime[2];
		bool _updateFlag;
	};
}
