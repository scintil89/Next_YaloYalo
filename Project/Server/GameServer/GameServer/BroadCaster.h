#pragma once

#include <memory>
#include <functional>
#include <map>
#include <list>
#include <set>

namespace LibNet
{
	class PacketQueue;
	struct PacketInfo;
}

namespace LibLogic
{
	class Observer;
	static const int32_t ALL_SESSION = -1;
	// std::set<packet id, session index>
	using Subject = std::pair<int32_t, int32_t>;
	using ObserverList = std::set< std::shared_ptr<Observer> >;

	class BroadCaster
	{
	public:
		BroadCaster() {}
		~BroadCaster() {}

		void		FeedPacketToObservers(std::shared_ptr<LibNet::PacketInfo> packetInfo);
		// subject : {packet id, session id}
		void		Subscribe(Subject subject, std::shared_ptr<Observer> observer);
		void		UnSubscribe(Subject subject,std::shared_ptr<Observer> observer);

	private:
	public:
	private:
		std::map<Subject,ObserverList> _observerQueues;
	};
}