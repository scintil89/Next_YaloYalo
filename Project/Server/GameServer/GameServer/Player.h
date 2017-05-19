#pragma once
#include <string>
#include "Define.h"

namespace LibLogic
{
	struct Player
	{
		Player(int sessionId, std::string& nickName, DEF::PlayerSide playerSide, int yalo) :
			_sessionId(sessionId),
			_nickName(nickName),
			_playerSide(playerSide),
			_currentYalo(yalo),
			_gameReady(false)
		{}

		int					_sessionId;
		std::string			_nickName;
		DEF::PlayerSide		_playerSide;
		int			_currentYalo;
		bool		_gameReady = false;
	};
}