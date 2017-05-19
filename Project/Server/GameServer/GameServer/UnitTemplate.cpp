#include "UnitTemplate.h"
#include <string>

#include "Unit.h"
#include "King.h"
#include "Arrow.h"
#include "Prince.h"
#include "Sparky.h"
#include "GoblinBarrel.h"

#include "UnitState.h"
#include "UnitState_Nothing.h"
#include "UnitState_Summoning.h"

#include "Match.h"
#include "UnitManager.h"

namespace LibLogic
{
	UnitTemplate* UnitTemplate::_instance = nullptr;

	UnitTemplate::UnitTemplate()
	{
		ZeroMemory(&_emptyObject, sizeof(_emptyObject));
	}

	UnitTemplate* UnitTemplate::GetInstance()
	{
		if (_instance == nullptr)
		{
			_instance = new UnitTemplate();
		}

		return _instance;
	}

	const UnitSpec& UnitTemplate::GetUnitSpec(const std::string& unitName)
	{
		if (_unitSpecs.find(unitName) == _unitSpecs.end())
		{
			return _emptyObject;
		}

		return _unitSpecs[unitName];
	}

	std::shared_ptr<LibLogic::Unit> UnitTemplate::CreateUnit(const std::string& un, DEF::PlayerSide ps, DEF::Vec2 pos, int idx, Match* match, UnitManager* um)
	{
		auto& us = GetUnitSpec(un);
		return us.createWithState(us, ps, pos, idx, match, um);
	}

	void UnitTemplate::SetCtorAndState()
	{
		for (auto& pair : _unitSpecs)
		{
			auto& us = pair.second;
			if (us.unitName == SWORD_MAN)
			{
				us.createWithState = [](const UnitSpec& us, DEF::PlayerSide ps, DEF::Vec2 pos, int idx, Match* match, UnitManager* um)
				{
					auto u = std::make_shared<Unit>(us, ps, pos, idx, match, um);
					u->ChangeState<UnitState_Summoning>();
					return u;
				};
			}
			else if (us.unitName == BOW_MAN)
			{
				us.createWithState = [](const UnitSpec& us, DEF::PlayerSide ps, DEF::Vec2 pos, int idx, Match* match, UnitManager* um)
				{
					auto u = std::make_shared<Unit>(us, ps, pos, idx, match, um);
					u->ChangeState<UnitState_Summoning>();
					return u;
				};
			}
			else if (us.unitName == NEXUS)
			{
				us.createWithState = [](const UnitSpec& us, DEF::PlayerSide ps, DEF::Vec2 pos, int idx, Match* match, UnitManager* um)
				{
					auto u = std::make_shared<Unit>(us, ps, pos, idx, match, um);
					u->ChangeState<UnitState_Nothing>();
					return u;
				};
			}
			else if (us.unitName == KING)
			{
				us.createWithState = [](const UnitSpec& us, DEF::PlayerSide ps, DEF::Vec2 pos, int idx, Match* match, UnitManager* um)
				{
					auto u = std::make_shared<King>(us, ps, pos, idx, match, um);
					u->ChangeState<UnitState_Nothing>();
					return u;
				};
			}
			else if (us.unitName == ARROW)
			{
				us.createWithState = [](const UnitSpec& us, DEF::PlayerSide ps, DEF::Vec2 pos, int idx, Match* match, UnitManager* um)
				{
					auto u = std::make_shared<Arrow>(us, ps, pos, idx, match, um);
					u->ChangeState<UnitState_Nothing>();
					return u;
				};
			}
			else if (us.unitName == PRINCE)
			{
				us.createWithState = [](const UnitSpec& us, DEF::PlayerSide ps, DEF::Vec2 pos, int idx, Match* match, UnitManager* um)
				{
					auto u = std::make_shared<Prince>(us, ps, pos, idx, match, um);
					u->ChangeState<UnitState_Summoning>();
					return u;
				};
			}
			else if (us.unitName == SPARKY)
			{
				us.createWithState = [](const UnitSpec& us, DEF::PlayerSide ps, DEF::Vec2 pos, int idx, Match* match, UnitManager* um)
				{
					auto u = std::make_shared<Sparky>(us, ps, pos, idx, match, um);
					u->ChangeState<UnitState_Summoning>();
					return u;
				};
			}
			else if (us.unitName == GOBLIN_BARREL)
			{
				us.createWithState = [](const UnitSpec& us, DEF::PlayerSide ps, DEF::Vec2 pos, int idx, Match* match, UnitManager* um)
				{
					auto u = std::make_shared<GoblinBarrel>(us, ps, pos, idx, match, um);
					u->ChangeState<UnitState_Summoning>();
					return u;
				};
			}
		}
	}
}
