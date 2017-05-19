#pragma once
#include <unordered_map>
#include <functional>
#include <memory>
#include "Define.h"

namespace LibLogic
{
	class Unit;
	class Match;
	class UnitManager;

	struct UnitSpec
	{
	public:
		std::string unitName;
		int yaloCost;
		int hp;
		bool isMagic;
		float summonTime;
		float sightRange;
		float attackRange;
		int attackPower;
		float attackFreq;
		float attackDelay;
		float moveSpeed;
		std::function<std::shared_ptr<Unit>(const UnitSpec&, DEF::PlayerSide, DEF::Vec2, int, Match*, UnitManager*)> createWithState;
	};

	class UnitTemplate
	{
		friend class DataLoader;

	public:
		static UnitTemplate* GetInstance();
		~UnitTemplate() = default;
		const UnitSpec& GetUnitSpec(const std::string& unitName);
		std::shared_ptr<Unit> CreateUnit(const std::string& un, DEF::PlayerSide ps, DEF::Vec2 pos, int idx, Match* match, UnitManager* um);

	private:
		auto& GetUnitSpecs() { return _unitSpecs; }	
		//Call this after load data
		void SetCtorAndState();
		UnitTemplate();
		static UnitTemplate* _instance;

	private:
		const std::string SWORD_MAN = "SwordMan";
		const std::string BOW_MAN = "BowMan";
		const std::string NEXUS = "Nexus";
		const std::string KING = "King";
		const std::string ARROW = "Arrow";
		const std::string PRINCE = "Prince";
		const std::string SPARKY = "Sparky";
		const std::string GOBLIN_BARREL = "GoblinBarrel";

	private:
		std::unordered_map<std::string, UnitSpec> _unitSpecs;
		UnitSpec _emptyObject;
	};
}