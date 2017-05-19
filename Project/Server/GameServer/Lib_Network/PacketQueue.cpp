
#include "LockGuard.h"

#include "PacketQueue.h"


namespace LibNet
{
	
	PacketQueue::PacketQueue()
	{
	}


	PacketQueue::~PacketQueue()
	{
	}

	std::shared_ptr<PacketInfo> PacketQueue::ReadFront()
	{
		LockGuard criticalSection(_mutex);
		if (_packetDequeue.empty())
		{
			return nullptr;
		}
		return _packetDequeue.front();
	}

	void PacketQueue::PopFront()
	{
		LockGuard criticalSection(_mutex);
		if (_packetDequeue.empty())
		{
			return;
		}
		_packetDequeue.pop_front();
	}

	void PacketQueue::PushBack(std::shared_ptr<PacketInfo> recvPacekt)
	{	
// 		auto newBody = new char[recvPacekt->_packetBodySize];
// 		memcpy_s(newBody, recvPacekt->_packetBodySize, recvPacekt->_data, recvPacekt->_packetBodySize);

		LockGuard criticalSection(_mutex);
		_packetDequeue.push_back(recvPacekt);
	}

	bool PacketQueue::isEmpty()
	{
		LockGuard criticalSection(_mutex);
		return _packetDequeue.empty();
	}

}