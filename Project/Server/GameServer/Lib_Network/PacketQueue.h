#pragma once

#include <mutex>
#include <deque>

#include "NetPacketInfo.h"

namespace LibNet
{
	class PacketQueue
	{
	public:
		PacketQueue();
		~PacketQueue();

		std::shared_ptr<PacketInfo>		ReadFront();
		void							PopFront();
		void							PushBack(std::shared_ptr<PacketInfo> recvPacekt);
		bool							isEmpty();

	private:
		std::deque<std::shared_ptr<PacketInfo>>	_packetDequeue;
		std::mutex						_mutex;
	};
}