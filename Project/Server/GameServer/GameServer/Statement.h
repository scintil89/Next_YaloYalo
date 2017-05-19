#pragma once

#include <functional>

namespace LibLogic
{
	class Statement
	{
	public:
		Statement() = default;
		explicit Statement(std::function<void()> statement, bool isTriggered = false);
		void Execute();
		void ExecuteOnce();
		void ExecuteIfTriggered();
		void Trigger();
	private:
		std::function<void()> _statement;
		bool _isTriggered;
		bool _hasExecuted;
	};
}