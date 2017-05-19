#include "Clock.h"

namespace LibLogic
{
	Clock::Clock(float requiredTime) :
		_completeTime(requiredTime), _elaspedTime(0.0f), _isComplete(false)
	{
	}

	void Clock::Tick(float dt)
	{
		if (_isComplete) {
			return;
		}

		_elaspedTime += dt;
		if (_elaspedTime > _completeTime) {
			_elaspedTime = 0.0f;
			_isComplete = true;
		}
	}

	bool Clock::IsComplete()
	{
		return _isComplete;
	}

	void Clock::Reset()
	{
		_elaspedTime = 0.0f;
		_isComplete = false;
	}
}