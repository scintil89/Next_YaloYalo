#include "json/json.h"

#include "PacketQueue.h"
#include "NetPacketInfo.h"

#include "Observer.h"

namespace LibLogic
{

	void Observer::StorePacket(std::shared_ptr<LibNet::PacketInfo> packetInfo)
	{
		_recvPacketQueue->PushBack(packetInfo);
		_packetCount.fetch_add(1);
	}

	Observer::Observer()
	{
		_recvPacketQueue = std::make_shared<LibNet::PacketQueue>();
	}

}