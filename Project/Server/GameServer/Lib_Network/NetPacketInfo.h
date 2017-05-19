#pragma once

namespace LibNet
{
	// temp packet header info
#define ID_PACKET_CLOSE_SESSION -1
	struct PacketHeader
	{
		int32_t		_id;
		int32_t		_bodySize;
	};

	const short PACKET_HEADER_SIZE = sizeof(PacketHeader);

	struct PacketInfo
	{
		PacketInfo(int32_t sessionIndex, int32_t packetId, int32_t packetBodySize, char* data, bool autoDelete)
			:_sessionIndex(sessionIndex), _packetId(packetId), _packetBodySize(packetBodySize), _data(data), _autoDelete(autoDelete)
		{}
		~PacketInfo()
		{
			if(_autoDelete)
				delete[] _data;
		}
		bool		_autoDelete = false;
		int32_t		_sessionIndex = 0;
		int32_t		_packetId = 0;
		int32_t		_packetBodySize = 0;
		char*		_data;
	};
}