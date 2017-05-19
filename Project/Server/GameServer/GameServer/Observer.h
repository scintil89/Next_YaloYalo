#pragma once

#include <memory>
#include <atomic>

namespace LibNet
{
	struct PacketInfo;
	class PacketQueue;
}

namespace LibLogic
{
	class Observer : public std::enable_shared_from_this<Observer>
	{
	public:
		Observer();
		virtual ~Observer() {}

		virtual void	StorePacket(std::shared_ptr<LibNet::PacketInfo> packetInfo);
	private:

	public:

	protected:
		std::shared_ptr<LibNet::PacketQueue>	_recvPacketQueue;
		std::atomic<int>						_packetCount = 0;

	private:

	};

}

