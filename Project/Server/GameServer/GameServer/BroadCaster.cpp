#include <stdexcept>

#include "Logger.h"
#include "NetPacketInfo.h"
#include "Observer.h"

#include "BroadCaster.h"

using namespace LibNet;

namespace LibLogic
{
	void BroadCaster::FeedPacketToObservers(std::shared_ptr<LibNet::PacketInfo> packetInfo)
	{
		// �ش� ���ǿ� �����ִ� ������ �����ش�.
		auto iter = _observerQueues.find(Subject{ packetInfo->_packetId,packetInfo->_sessionIndex });
		if (iter != _observerQueues.end())
		{
			for (auto& observer : iter->second)
			{
				observer->StorePacket(packetInfo);
			}
		}
		// ALL_SESSION���� ����� ������״� ������ �����ش�.
		iter = _observerQueues.find(Subject{ packetInfo->_packetId,ALL_SESSION });
		if (iter != _observerQueues.end())
		{
			for (auto& observer : iter->second)
			{
				observer->StorePacket(packetInfo);
			}
		}


	}

	void BroadCaster::Subscribe(Subject subject, std::shared_ptr<Observer> observer)
	{
		// if there is no list that has been made, insert new one
		if (_observerQueues.find(subject) == _observerQueues.end())
		{
			ObserverList newList;
			_observerQueues.emplace(subject, std::move(newList));
		}

		// add observer queue who has interest in the subject
		_observerQueues.at(subject).emplace(observer);
	}

	void BroadCaster::UnSubscribe(Subject subject, std::shared_ptr<Observer> observer)
	{
		auto iterList = _observerQueues.find(subject);
		// there is no subsriber for the subject
		if (iterList == _observerQueues.end())
		{
			LibLogger::cWarning << "BroadCaster::Unsubscribe() : there is no subscriber to the subject" << std::endl;
		}
		else
		{
			auto& observerList = iterList->second;
			auto iterObserver = observerList.find(observer);
			// the observer is not subscriber to the subject
			if (iterObserver == observerList.end())
			{
				LibLogger::cWarning << "BroadCaster::Unsubscribe() : the observer is not subscriber to the subject" << std::endl;
			}
			// process unsubscribe
			else
			{
				// �ش� �����ڸ� �����
				LibLogger::cInfo << "" << std::endl;
				observerList.erase(iterObserver);
			}
		}
	}
}