#pragma once

#include <memory>
#include "BroadCaster.h"

namespace LibNet
{
	class PacketQueue;
}

namespace LibLogic
{
	class BroadCaster;
	class MatchMother;
	class App
	{
	public:
		App();
		~App() {}

		void Run();

	private:

	public:

	private:
		static bool									_isAppRunning;
		bool										_isAppEnd = false;

		std::shared_ptr<LibLogic::BroadCaster>		_broadCaster;
		std::shared_ptr<LibLogic::MatchMother>		_mother;
		std::weak_ptr<LibNet::PacketQueue>			_recvPacketQueue;
		std::weak_ptr<LibNet::PacketQueue>			_sendPacketQueue;
	};
}