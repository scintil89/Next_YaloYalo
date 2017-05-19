#pragma once

#include <string>
#include <vector>
#include "..\..\Server\GameServer\GameServer\json\json.h"

namespace Packets
{
	class IJsonSerializable
	{
	public:
		virtual ~IJsonSerializable( void ) {};
		virtual void Serialize( Json::Value& root ) =0;
		virtual void Deserialize( Json::Value& root ) =0;
	};

	class CJsonSerializer
	{
	public:
		static bool Serialize( IJsonSerializable* pObj, std::string& output );
		static bool Deserialize( IJsonSerializable* pObj, std::string& input );

	private:
		CJsonSerializer( void ) {};
	};

	// 매치 신청 패킷
	class PACKET_MATCH_JOIN_REQ : public IJsonSerializable
	{
	public:
		PACKET_MATCH_JOIN_REQ( void ) {}
		virtual ~PACKET_MATCH_JOIN_REQ( void ) {}
		virtual void Serialize ( Json::Value& root );
		virtual void Deserialize( Json::Value& root );

	    // 기기 고유값 (클라이언트 id 역할)
	    // 게임에서 사용할 닉네임
	    // 매치서버가 알려준 방 번호
		std::string					_uuid;
		std::string					_nickname;
		int							_roomNum;
	};

	// 매치 신청 결과 패킷
	class PACKET_MATCH_JOIN_RES : public IJsonSerializable
	{
	public:
		PACKET_MATCH_JOIN_RES( void ) {}
		virtual ~PACKET_MATCH_JOIN_RES( void ) {}
		virtual void Serialize ( Json::Value& root );
		virtual void Deserialize( Json::Value& root );

		bool						_isSuccess;
		std::string					_opponentNickname;
	};

	// 게임 시작하기 위한 정보를 서버에서 알려준다
	class PACKET_MATCH_START_NTF : public IJsonSerializable
	{
	public:
		PACKET_MATCH_START_NTF( void ) {}
		virtual ~PACKET_MATCH_START_NTF( void ) {}
		virtual void Serialize ( Json::Value& root );
		virtual void Deserialize( Json::Value& root );

	    // true가 player1, false가 player2
		bool						_playerSide;
		float						_yaloChargeTime;
		int							_yaloStartNum;
		int							_maxYaloNum;
		float						_mapSizeX;
		float						_mapSizeY;
		float						_leftBridgeRectX;
		float						_leftBridgeRectY;
		float						_rightBridgeRectX;
		float						_rightBridgeRectY;
		float						_bridgeWidth;
		float						_bridgeHeight;
	};

	class PACKET_CLIENT_GAME_READY_REQ : public IJsonSerializable
	{
	public:
		PACKET_CLIENT_GAME_READY_REQ( void ) {}
		virtual ~PACKET_CLIENT_GAME_READY_REQ( void ) {}
		virtual void Serialize ( Json::Value& root );
		virtual void Deserialize( Json::Value& root );

		bool						_gameReady;
	};

	class PACKET_UNIT_SUMMON_REQ : public IJsonSerializable
	{
	public:
		PACKET_UNIT_SUMMON_REQ( void ) {}
		virtual ~PACKET_UNIT_SUMMON_REQ( void ) {}
		virtual void Serialize ( Json::Value& root );
		virtual void Deserialize( Json::Value& root );

		std::string					_unitName;
		float						_posX;
		float						_posY;
	};

	class PACKET_UNIT_SUMMON_NTF : public IJsonSerializable
	{
	public:
		PACKET_UNIT_SUMMON_NTF( void ) {}
		virtual ~PACKET_UNIT_SUMMON_NTF( void ) {}
		virtual void Serialize ( Json::Value& root );
		virtual void Deserialize( Json::Value& root );

		bool						_playerSide;
		std::string					_unitName;
		int							_unitNum;
		float						_posX;
		float						_posY;
		int							_hp;
	};

	class PACKET_UNIT_MOVE_TO_TARGET_NTF : public IJsonSerializable
	{
	public:
		PACKET_UNIT_MOVE_TO_TARGET_NTF( void ) {}
		virtual ~PACKET_UNIT_MOVE_TO_TARGET_NTF( void ) {}
		virtual void Serialize ( Json::Value& root );
		virtual void Deserialize( Json::Value& root );

		int							_unitNum;
		int							_targetNum;
		float						_currentPosX;
		float						_currentPosY;
		float						_speed;
	};

	class PACKET_UNIT_MOVE_TO_WAYPOINT_NTF : public IJsonSerializable
	{
	public:
		PACKET_UNIT_MOVE_TO_WAYPOINT_NTF( void ) {}
		virtual ~PACKET_UNIT_MOVE_TO_WAYPOINT_NTF( void ) {}
		virtual void Serialize ( Json::Value& root );
		virtual void Deserialize( Json::Value& root );

		int							_unitNum;
		float						_moveValueX;
		float						_moveValueY;
		float						_wayPointX;
		float						_wayPointY;
	};

	class PACKET_UNIT_ATTACK_NTF : public IJsonSerializable
	{
	public:
		PACKET_UNIT_ATTACK_NTF( void ) {}
		virtual ~PACKET_UNIT_ATTACK_NTF( void ) {}
		virtual void Serialize ( Json::Value& root );
		virtual void Deserialize( Json::Value& root );

		int							_unitNum;
		int							_targetNum;
		float						_attackDelay;
	};

	class PACKET_UNIT_HP_CHANGED : public IJsonSerializable
	{
	public:
		PACKET_UNIT_HP_CHANGED( void ) {}
		virtual ~PACKET_UNIT_HP_CHANGED( void ) {}
		virtual void Serialize ( Json::Value& root );
		virtual void Deserialize( Json::Value& root );

		int							_unitNum;
		int							_hp;
	};

	class PACKET_UNIT_DIE_NTF : public IJsonSerializable
	{
	public:
		PACKET_UNIT_DIE_NTF( void ) {}
		virtual ~PACKET_UNIT_DIE_NTF( void ) {}
		virtual void Serialize ( Json::Value& root );
		virtual void Deserialize( Json::Value& root );

		int							_unitNum;
	};

	class PACKET_YALO_CHANGED_NTF : public IJsonSerializable
	{
	public:
		PACKET_YALO_CHANGED_NTF( void ) {}
		virtual ~PACKET_YALO_CHANGED_NTF( void ) {}
		virtual void Serialize ( Json::Value& root );
		virtual void Deserialize( Json::Value& root );

		bool						_playerSide;
		int							_currentYalo;
	};

	class PACKET_GAME_RESULT_NTF : public IJsonSerializable
	{
	public:
		PACKET_GAME_RESULT_NTF( void ) {}
		virtual ~PACKET_GAME_RESULT_NTF( void ) {}
		virtual void Serialize ( Json::Value& root );
		virtual void Deserialize( Json::Value& root );

	    // true가 player1, false가 player2
		bool						_winPlayerSide;
	};

	class PACKET_CONNECT_CLOSE_NTF : public IJsonSerializable
	{
	public:
		PACKET_CONNECT_CLOSE_NTF( void ) {}
		virtual ~PACKET_CONNECT_CLOSE_NTF( void ) {}
		virtual void Serialize ( Json::Value& root );
		virtual void Deserialize( Json::Value& root );

		int							_sessionIdx;
	};

	class PACKET_UNIT_SPECIAL_NTF : public IJsonSerializable
	{
	public:
		PACKET_UNIT_SPECIAL_NTF( void ) {}
		virtual ~PACKET_UNIT_SPECIAL_NTF( void ) {}
		virtual void Serialize ( Json::Value& root );
		virtual void Deserialize( Json::Value& root );

		float						_fval1;
		float						_fval2;
		int							_ival;
		int							_unitNum;
	};

	enum PacketId
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