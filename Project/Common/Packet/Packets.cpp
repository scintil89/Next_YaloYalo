#include "Packets.h"

namespace Packets
{
	bool CJsonSerializer::Serialize( IJsonSerializable* pObj, std::string& output )
	{
		if (pObj == NULL)
			return false;

		Json::Value serializeRoot;
		pObj->Serialize(serializeRoot);

		Json::StyledWriter writer;
		output = writer.write( serializeRoot );

		return true;
	}

	bool CJsonSerializer::Deserialize( IJsonSerializable* pObj, std::string& input )
	{
		if (pObj == NULL)
			return false;

		Json::Value deserializeRoot;
		Json::Reader reader;

		if ( !reader.parse(input, deserializeRoot) )
			return false;

		pObj->Deserialize(deserializeRoot);

		return true;
	}

	void PACKET_MATCH_JOIN_REQ::Serialize( Json::Value& root )
	{
		root["_uuid"] = _uuid;
		root["_nickname"] = _nickname;
		root["_roomNum"] = _roomNum;
	}

	void PACKET_MATCH_JOIN_REQ::Deserialize( Json::Value& root )
	{
		_uuid = root.get("_uuid", "").asString();
		_nickname = root.get("_nickname", "").asString();
		_roomNum = root.get("_roomNum", 0).asInt();
	}

	void PACKET_MATCH_JOIN_RES::Serialize( Json::Value& root )
	{
		root["_isSuccess"] = _isSuccess;
		root["_opponentNickname"] = _opponentNickname;
	}

	void PACKET_MATCH_JOIN_RES::Deserialize( Json::Value& root )
	{
		_isSuccess = root.get("_isSuccess", false).asBool();
		_opponentNickname = root.get("_opponentNickname", "").asString();
	}

	void PACKET_MATCH_START_NTF::Serialize( Json::Value& root )
	{
		root["_playerSide"] = _playerSide;
		root["_yaloChargeTime"] = _yaloChargeTime;
		root["_yaloStartNum"] = _yaloStartNum;
		root["_maxYaloNum"] = _maxYaloNum;
		root["_mapSizeX"] = _mapSizeX;
		root["_mapSizeY"] = _mapSizeY;
		root["_leftBridgeRectX"] = _leftBridgeRectX;
		root["_leftBridgeRectY"] = _leftBridgeRectY;
		root["_rightBridgeRectX"] = _rightBridgeRectX;
		root["_rightBridgeRectY"] = _rightBridgeRectY;
		root["_bridgeWidth"] = _bridgeWidth;
		root["_bridgeHeight"] = _bridgeHeight;
	}

	void PACKET_MATCH_START_NTF::Deserialize( Json::Value& root )
	{
		_playerSide = root.get("_playerSide", false).asBool();
		_yaloChargeTime = root.get("_yaloChargeTime", 0.0).asDouble();
		_yaloStartNum = root.get("_yaloStartNum", 0).asInt();
		_maxYaloNum = root.get("_maxYaloNum", 0).asInt();
		_mapSizeX = root.get("_mapSizeX", 0.0).asDouble();
		_mapSizeY = root.get("_mapSizeY", 0.0).asDouble();
		_leftBridgeRectX = root.get("_leftBridgeRectX", 0.0).asDouble();
		_leftBridgeRectY = root.get("_leftBridgeRectY", 0.0).asDouble();
		_rightBridgeRectX = root.get("_rightBridgeRectX", 0.0).asDouble();
		_rightBridgeRectY = root.get("_rightBridgeRectY", 0.0).asDouble();
		_bridgeWidth = root.get("_bridgeWidth", 0.0).asDouble();
		_bridgeHeight = root.get("_bridgeHeight", 0.0).asDouble();
	}

	void PACKET_CLIENT_GAME_READY_REQ::Serialize( Json::Value& root )
	{
		root["_gameReady"] = _gameReady;
	}

	void PACKET_CLIENT_GAME_READY_REQ::Deserialize( Json::Value& root )
	{
		_gameReady = root.get("_gameReady", false).asBool();
	}

	void PACKET_UNIT_SUMMON_REQ::Serialize( Json::Value& root )
	{
		root["_unitName"] = _unitName;
		root["_posX"] = _posX;
		root["_posY"] = _posY;
	}

	void PACKET_UNIT_SUMMON_REQ::Deserialize( Json::Value& root )
	{
		_unitName = root.get("_unitName", "").asString();
		_posX = root.get("_posX", 0.0).asDouble();
		_posY = root.get("_posY", 0.0).asDouble();
	}

	void PACKET_UNIT_SUMMON_NTF::Serialize( Json::Value& root )
	{
		root["_playerSide"] = _playerSide;
		root["_unitName"] = _unitName;
		root["_unitNum"] = _unitNum;
		root["_posX"] = _posX;
		root["_posY"] = _posY;
		root["_hp"] = _hp;
	}

	void PACKET_UNIT_SUMMON_NTF::Deserialize( Json::Value& root )
	{
		_playerSide = root.get("_playerSide", false).asBool();
		_unitName = root.get("_unitName", "").asString();
		_unitNum = root.get("_unitNum", 0).asInt();
		_posX = root.get("_posX", 0.0).asDouble();
		_posY = root.get("_posY", 0.0).asDouble();
		_hp = root.get("_hp", 0).asInt();
	}

	void PACKET_UNIT_MOVE_TO_TARGET_NTF::Serialize( Json::Value& root )
	{
		root["_unitNum"] = _unitNum;
		root["_targetNum"] = _targetNum;
		root["_currentPosX"] = _currentPosX;
		root["_currentPosY"] = _currentPosY;
		root["_speed"] = _speed;
	}

	void PACKET_UNIT_MOVE_TO_TARGET_NTF::Deserialize( Json::Value& root )
	{
		_unitNum = root.get("_unitNum", 0).asInt();
		_targetNum = root.get("_targetNum", 0).asInt();
		_currentPosX = root.get("_currentPosX", 0.0).asDouble();
		_currentPosY = root.get("_currentPosY", 0.0).asDouble();
		_speed = root.get("_speed", 0.0).asDouble();
	}

	void PACKET_UNIT_MOVE_TO_WAYPOINT_NTF::Serialize( Json::Value& root )
	{
		root["_unitNum"] = _unitNum;
		root["_moveValueX"] = _moveValueX;
		root["_moveValueY"] = _moveValueY;
		root["_wayPointX"] = _wayPointX;
		root["_wayPointY"] = _wayPointY;
	}

	void PACKET_UNIT_MOVE_TO_WAYPOINT_NTF::Deserialize( Json::Value& root )
	{
		_unitNum = root.get("_unitNum", 0).asInt();
		_moveValueX = root.get("_moveValueX", 0.0).asDouble();
		_moveValueY = root.get("_moveValueY", 0.0).asDouble();
		_wayPointX = root.get("_wayPointX", 0.0).asDouble();
		_wayPointY = root.get("_wayPointY", 0.0).asDouble();
	}

	void PACKET_UNIT_ATTACK_NTF::Serialize( Json::Value& root )
	{
		root["_unitNum"] = _unitNum;
		root["_targetNum"] = _targetNum;
		root["_attackDelay"] = _attackDelay;
	}

	void PACKET_UNIT_ATTACK_NTF::Deserialize( Json::Value& root )
	{
		_unitNum = root.get("_unitNum", 0).asInt();
		_targetNum = root.get("_targetNum", 0).asInt();
		_attackDelay = root.get("_attackDelay", 0.0).asDouble();
	}

	void PACKET_UNIT_HP_CHANGED::Serialize( Json::Value& root )
	{
		root["_unitNum"] = _unitNum;
		root["_hp"] = _hp;
	}

	void PACKET_UNIT_HP_CHANGED::Deserialize( Json::Value& root )
	{
		_unitNum = root.get("_unitNum", 0).asInt();
		_hp = root.get("_hp", 0).asInt();
	}

	void PACKET_UNIT_DIE_NTF::Serialize( Json::Value& root )
	{
		root["_unitNum"] = _unitNum;
	}

	void PACKET_UNIT_DIE_NTF::Deserialize( Json::Value& root )
	{
		_unitNum = root.get("_unitNum", 0).asInt();
	}

	void PACKET_YALO_CHANGED_NTF::Serialize( Json::Value& root )
	{
		root["_playerSide"] = _playerSide;
		root["_currentYalo"] = _currentYalo;
	}

	void PACKET_YALO_CHANGED_NTF::Deserialize( Json::Value& root )
	{
		_playerSide = root.get("_playerSide", false).asBool();
		_currentYalo = root.get("_currentYalo", 0).asInt();
	}

	void PACKET_GAME_RESULT_NTF::Serialize( Json::Value& root )
	{
		root["_winPlayerSide"] = _winPlayerSide;
	}

	void PACKET_GAME_RESULT_NTF::Deserialize( Json::Value& root )
	{
		_winPlayerSide = root.get("_winPlayerSide", false).asBool();
	}

	void PACKET_CONNECT_CLOSE_NTF::Serialize( Json::Value& root )
	{
		root["_sessionIdx"] = _sessionIdx;
	}

	void PACKET_CONNECT_CLOSE_NTF::Deserialize( Json::Value& root )
	{
		_sessionIdx = root.get("_sessionIdx", 0).asInt();
	}

	void PACKET_UNIT_SPECIAL_NTF::Serialize( Json::Value& root )
	{
		root["_fval1"] = _fval1;
		root["_fval2"] = _fval2;
		root["_ival"] = _ival;
		root["_unitNum"] = _unitNum;
	}

	void PACKET_UNIT_SPECIAL_NTF::Deserialize( Json::Value& root )
	{
		_fval1 = root.get("_fval1", 0.0).asDouble();
		_fval2 = root.get("_fval2", 0.0).asDouble();
		_ival = root.get("_ival", 0).asInt();
		_unitNum = root.get("_unitNum", 0).asInt();
	}

}