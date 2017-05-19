#pragma once

#include <mutex>

namespace LibNet
{
	class LockGuard
	{
	public:
		LockGuard(std::mutex& lock) : _lock(lock)
		{
			_lock.lock();
		}

		~LockGuard()
		{
			_lock.unlock();
		}

	private:
		std::mutex& _lock;
	};

}