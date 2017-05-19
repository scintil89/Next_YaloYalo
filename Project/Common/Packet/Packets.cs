namespace Packets
{
	// 매치 신청 패킷
	public class PACKET_MATCH_JOIN_REQ
{
	    // 기기 고유값 (클라이언트 id 역할)
	    // 게임에서 사용할 닉네임
	    // 매치서버가 알려준 방 번호
	public string				_uuid;
	public string				_nickname;
	public int					_roomNum;
	}

	// 매치 신청 결과 패킷
	public class PACKET_MATCH_JOIN_RES
{
	public bool				_isSuccess;
	public string				_opponentNickname;
	}

	// 게임 시작하기 위한 정보를 서버에서 알려준다
	public class PACKET_MATCH_START_NTF
{
	    // true가 player1, false가 player2
	public bool				_playerSide;
	public float				_yaloChargeTime;
	public int					_yaloStartNum;
	public int					_maxYaloNum;
	public float				_mapSizeX;
	public float				_mapSizeY;
	public float				_leftBridgeRectX;
	public float				_leftBridgeRectY;
	public float				_rightBridgeRectX;
	public float				_rightBridgeRectY;
	public float				_bridgeWidth;
	public float				_bridgeHeight;
	}

	public class PACKET_CLIENT_GAME_READY_REQ
{
	public bool				_gameReady;
	}

	public class PACKET_UNIT_SUMMON_REQ
{
	public string				_unitName;
	public float				_posX;
	public float				_posY;
	}

	public class PACKET_UNIT_SUMMON_NTF
{
	public bool				_playerSide;
	public string				_unitName;
	public int					_unitNum;
	public float				_posX;
	public float				_posY;
	public int					_hp;
	}

	public class PACKET_UNIT_MOVE_TO_TARGET_NTF
{
	public int					_unitNum;
	public int					_targetNum;
	public float				_currentPosX;
	public float				_currentPosY;
	public float				_speed;
	}

	public class PACKET_UNIT_MOVE_TO_WAYPOINT_NTF
{
	public int					_unitNum;
	public float				_moveValueX;
	public float				_moveValueY;
	public float				_wayPointX;
	public float				_wayPointY;
	}

	public class PACKET_UNIT_ATTACK_NTF
{
	public int					_unitNum;
	public int					_targetNum;
	public float				_attackDelay;
	}

	public class PACKET_UNIT_HP_CHANGED
{
	public int					_unitNum;
	public int					_hp;
	}

	public class PACKET_UNIT_DIE_NTF
{
	public int					_unitNum;
	}

	public class PACKET_YALO_CHANGED_NTF
{
	public bool				_playerSide;
	public int					_currentYalo;
	}

	public class PACKET_GAME_RESULT_NTF
{
	    // true가 player1, false가 player2
	public bool				_winPlayerSide;
	}

	public class PACKET_CONNECT_CLOSE_NTF
{
	public int					_sessionIdx;
	}

	public class PACKET_UNIT_SPECIAL_NTF
{
	public float				_fval1;
	public float				_fval2;
	public int					_ival;
	public int					_unitNum;
	}

	public enum PacketId
	{
		ID_PACKET_MATCH_JOIN_REQ			= 101,
		ID_PACKET_MATCH_JOIN_RES			= 102,
		ID_PACKET_MATCH_START_NTF			= 103,
		ID_PACKET_CLIENT_GAME_READY_REQ			= 104,
		ID_PACKET_UNIT_SUMMON_REQ			= 105,
		ID_PACKET_UNIT_SUMMON_NTF			= 106,
		ID_PACKET_UNIT_MOVE_TO_TARGET_NTF			= 107,
		ID_PACKET_UNIT_MOVE_TO_WAYPOINT_NTF			= 108,
		ID_PACKET_UNIT_ATTACK_NTF			= 109,
		ID_PACKET_UNIT_HP_CHANGED			= 110,
		ID_PACKET_UNIT_DIE_NTF			= 111,
		ID_PACKET_YALO_CHANGED_NTF			= 112,
		ID_PACKET_GAME_RESULT_NTF			= 113,
		ID_PACKET_CONNECT_CLOSE_NTF			= 114,
		ID_PACKET_UNIT_SPECIAL_NTF			= 115,
	};
}