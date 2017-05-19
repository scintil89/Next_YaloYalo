#include "App.h"
#include "BroadCaster.h"
#include "MatchMother.h"

#include "IOCPManager.h"
#include "PacketQueue.h"
#include "NetPacketInfo.h"
#include "Packets.h"

#include "DataLoader.h"
#include "Logger.h"

using namespace LibNet;

namespace LibLogic
{
	bool App::_isAppRunning = false;

	App::App()
	{
		_broadCaster = std::make_shared<BroadCaster>();
		_mother = std::make_shared<MatchMother>(_broadCaster);
	}

	void App::Run()
	{
		if (_isAppRunning)
		{
			LibLogger::cError << "bad call : another app is already running" << std::endl;
			return;
		}
		_isAppRunning = true;
		
		// date
		LibLogger::cInfo << "2016_12_22 ver0.5" << std::endl;

		// run iocp manager
		_recvPacketQueue = IOCPManager::GetInstance()->GetRecvPacketQueue();
		IOCPManager::GetInstance()->Run();
		LibLogger::cInfo << "Network threads start ok" << std::endl;

		// launch a new mother thread
		auto motherThread = std::thread(std::bind(&MatchMother::Run, _mother.get()));
		LibLogger::cInfo << "Mother thread start ok" << std::endl;

		// load data
		DataLoader dl;
		dl.LoadData("UnitSpecs.json");

		while (_isAppRunning)
		{
			// get single packet from netlib
			// if there is no packet to process, 
			auto spt = _recvPacketQueue.lock();
			if (spt == nullptr)
			{
				// shared ptr expired
				LibLogger::cError << "shared ptr is expired" << std::endl;
				break;
			}
			else if (spt->isEmpty())
			{
				Sleep(0);
				continue;
			}
			// ReadFront() is safe in this moment because only this thread can process receive packet queue
			auto packet = spt->ReadFront();

			// broadcast income packet to all observers
			_broadCaster->FeedPacketToObservers(packet);

			// pop consumed packet
			// delete doesn't work here. it works when all observers complete the packet process
			spt->PopFront();
		}
		// TODO: cleanup memory and other process to end app cleanly
		_mother->Stop();
		motherThread.join();
	}

}