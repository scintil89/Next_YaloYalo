#include "Statement.h"

namespace LibLogic
{
	Statement::Statement(std::function<void()> statement, bool isTriggered) :
		_statement(statement), _isTriggered(isTriggered), _hasExecuted(false)
	{
	}

	void Statement::Execute()
	{
		_statement();
		_hasExecuted = true;
	}

	void Statement::ExecuteOnce()
	{
		if (!_hasExecuted) {
			_statement();
			_hasExecuted = true;
		}
	}

	void Statement::ExecuteIfTriggered()
	{
		if (_isTriggered) {
			_statement();
			_isTriggered = false;
		}
	}

	void Statement::Trigger()
	{
		_isTriggered = true;
	}
}