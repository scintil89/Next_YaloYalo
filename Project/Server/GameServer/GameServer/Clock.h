#pragma once

namespace LibLogic
{
	class Clock
	{
	public:
		Clock() = default;
		explicit Clock(float completeTime);
		void Tick(float dt);
		bool IsComplete();
		void Reset();

	private:
		bool _isComplete;
		float _elaspedTime;
		float _completeTime;
	};
}