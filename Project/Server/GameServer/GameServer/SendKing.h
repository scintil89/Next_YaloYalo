#pragma once

#include <memory>
#include <string>
#include "IOCPManager.h"
#include "NetPacketInfo.h"
#include "Packets.h"

namespace LibLogic
{
	class SendKing
	{
	public:
		template <class T>
		static void SendPacket(int sessionId, int packetId, T& packet)
		{
			auto jString = std::string{};
			Packets::CJsonSerializer::Serialize(&packet, jString);
			auto packetInfo = std::make_shared<LibNet::PacketInfo>(sessionId, packetId, static_cast<int32_t>(strlen(jString.c_str() + 1)), (char*)jString.c_str(), false);
			LibNet::IOCPManager::GetInstance()->PostSend(packetInfo);
		}
	private:
		SendKing() = default;
		~SendKing() = default;
	};
}