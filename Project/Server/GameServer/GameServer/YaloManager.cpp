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
		//��ΰ����� �����Ѵ�
		_players.at(playerSide)._currentYalo -= usedYalo;

		//��� ��� ����
		PACKET_YALO_CHANGED_NTF pkt;
		pkt._playerSide = playerSide;
		pkt._currentYalo = _players.at(playerSide)._currentYalo;
		_match->SendPacketToPlayer(playerSide, ID_PACKET_YALO_CHANGED_NTF, pkt);
	}

	void YaloManager::_Update(float dt, DEF::PlayerSide playerSide)
	{
		//�̹� �ִ�ġ ��ŭ ������ �ִ� ��� ������� �ʴ´�
		if (_players.at(playerSide)._currentYalo >= _yaloTable->maxYaloNumber)
		{
			return;
		}

		_elapsedTime[playerSide] += dt;

		//�����ð��� �Ǿ�����
		if (_elapsedTime[playerSide] >=
			_yaloTable->yaloChargeTime)
		{
			//��θ� ������Ų��
			_players.at(playerSide)._currentYalo++;
			
			//�����ð��� �ʱ�ȭ�Ѵ�
			_elapsedTime[playerSide] = 0.f;
			
			//�����Ѵ�
			PACKET_YALO_CHANGED_NTF pkt;
			pkt._playerSide = playerSide;
			pkt._currentYalo = _players.at(playerSide)._currentYalo;
			_match->SendPacketToPlayer(playerSide, ID_PACKET_YALO_CHANGED_NTF, pkt);
		}
	}
}