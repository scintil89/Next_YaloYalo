#include "YaloManager.h"
#include "Match.h"

namespace LibLogic
{
	YaloManager::YaloManager(Match* match, std::map<DEF::PlayerSide, Player>& players) :
		_match(match), _players(players), _updateFlag(false)
	{
		_yaloTable = YaloTable::GetInstance();

		_elapsedTime[DEF::PlayerSide::TOP] = 0.f;
		_elapsedTime[DEF::PlayerSide::BOTTOM] = 0.f;

		_players.at(DEF::PlayerSide::TOP)._currentYalo = _yaloTable->startYaloNumber;
		_players.at(DEF::PlayerSide::BOTTOM)._currentYalo = _yaloTable->startYaloNumber;
	}

	int YaloManager::GetCost(const std::string& unitName)
	{
		return _yaloTable->GetYaloCost(unitName);
	}

	void YaloManager::Update(float dt)
	{
		if (_updateFlag == true)
		{
			_Update(dt, DEF::PlayerSide::TOP);
			_Update(dt, DEF::PlayerSide::BOTTOM);
		}
	}

	void YaloManager::UseYalo(DEF::PlayerSide playerSide, int usedYalo)
	{
		//얄로개수를 갱신한다
		_players.at(playerSide)._currentYalo -= usedYalo;

		//얄로 사용 통지
		PACKET_YALO_CHANGED_NTF pkt;
		pkt._playerSide = playerSide;
		pkt._currentYalo = _players.at(playerSide)._currentYalo;
		_match->SendPacketToPlayer(playerSide, ID_PACKET_YALO_CHANGED_NTF, pkt);
	}

	void YaloManager::_Update(float dt, DEF::PlayerSide playerSide)
	{
		//이미 최대치 만큼 가지고 있는 경우 계산하지 않는다
		if (_players.at(playerSide)._currentYalo >= _yaloTable->maxYaloNumber)
		{
			return;
		}

		_elapsedTime[playerSide] += dt;

		//충전시간이 되었으면
		if (_elapsedTime[playerSide] >=
			_yaloTable->yaloChargeTime)
		{
			//얄로를 증가시킨다
			_players.at(playerSide)._currentYalo++;
			
			//누적시간을 초기화한다
			_elapsedTime[playerSide] = 0.f;
			
			//통지한다
			PACKET_YALO_CHANGED_NTF pkt;
			pkt._playerSide = playerSide;
			pkt._currentYalo = _players.at(playerSide)._currentYalo;
			_match->SendPacketToPlayer(playerSide, ID_PACKET_YALO_CHANGED_NTF, pkt);
		}
	}
}