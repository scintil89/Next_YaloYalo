#include "UnitManager.h"

//Units
#include "Unit.h"

//Networks
#include "Packets.h"
#include "SendKing.h"

//
#include "Match.h"
#include "UnitTemplate.h"
#include "Logger.h"

namespace LibLogic
{
	UnitManager::UnitManager(Match* match) :
		_match(match), _lastUnitIndex(0)
	{
		_unitSpecs = UnitTemplate::GetInstance();
	}

	std::shared_ptr<Unit> UnitManager::GetUnit(int unitNum)
	{
		auto elem = _unitList.find(unitNum);
		if (elem == _unitList.end())
		{
			return nullptr;
		}

		return elem->second;
	}

	bool UnitManager::SummonUnit(const std::string& unitName, const DEF::Vec2& position, const DEF::PlayerSide playerSide)
	{
		auto unit = CreateUnit(unitName, position, playerSide);

		if (unit == nullptr)
		{
			LibLogger::cWarning << "유닛 생성 실패 : [" << unitName << "]" << std::endl;
			return false;
		}

		if (_unitList.find(_lastUnitIndex) != _unitList.end())
		{
			LibLogger::cWarning << "유닛 아이디 중복 : [" << unitName << "]" << std::endl;
			return false;
		}

		// 소환 통지 
		NotifySummon(unitName, _lastUnitIndex, unit->GetHp(), position, playerSide);

		// 리스트에 추가
		_unitList[_lastUnitIndex] = unit;
		++_lastUnitIndex;

		return true;
	}

	void UnitManager::UpdateAllUnits(float dt)
	{
		for (auto& it : _unitList)
		{
			if (it.second == nullptr ||
				it.second->GetIsDead() == true)
			{
				_unitList.erase(it.first);
				continue;
			}

			it.second->Update(dt);
		}
	}

	// 조건식을 받아서 조건에 맞는 유닛들을 반환함.
	std::list<int>& UnitManager::FindUnitsByCondition(int callerNum, std::function<bool(Unit* callerObject, Unit* other)> condition)
	{
		_tempUnitList.clear();

		//유효하지 않은 caller
		if (_unitList.find(callerNum) == _unitList.end() ||
			_unitList[callerNum] == nullptr)
		{
			return std::move(_tempUnitList);
		}

		auto& callerObject = _unitList.at(callerNum);
		for (auto& it : _unitList)
		{
			auto& unitNum = it.first;
			auto& unitObject = it.second;
			if (condition(callerObject.get(), unitObject.get()))
			{
				_tempUnitList.push_back(unitNum);
			}
		}

		return std::move(_tempUnitList);
	}

	std::shared_ptr<Unit> UnitManager::CreateUnit(const std::string& unitName, const DEF::Vec2& position, const DEF::PlayerSide playerSide)
	{
		return _unitSpecs->CreateUnit(unitName, playerSide, position, _lastUnitIndex, _match, this);
	}
	
	void UnitManager::NotifySummon(const std::string& unitName, int unitNum, int hp, const DEF::Vec2& position, const DEF::PlayerSide playerSide)
	{
		auto summonNtf = Packets::PACKET_UNIT_SUMMON_NTF{};
		summonNtf._playerSide = (playerSide == DEF::PlayerSide::BOTTOM) ? false : true;
		summonNtf._posX = position._x;
		summonNtf._posY = position._y;
		summonNtf._unitName = unitName;
		summonNtf._unitNum = unitNum;
		summonNtf._hp = hp;
		
		_match->SendPacketToPlayer(DEF::PlayerSide::BOTTOM, Packets::PacketId::ID_PACKET_UNIT_SUMMON_NTF, summonNtf);
		_match->SendPacketToPlayer(DEF::PlayerSide::TOP, Packets::PacketId::ID_PACKET_UNIT_SUMMON_NTF, summonNtf);
	}
}