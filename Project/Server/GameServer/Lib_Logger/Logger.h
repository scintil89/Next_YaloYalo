#pragma once

#include <iostream>

namespace LibLogger
{
	class Logger
	{
	public:
		enum LogLevel
		{
			NONE,
			DEBUG,
			INFO,
			WARNING,
			ERR,
			FATAL
		};

	public:
		Logger(LogLevel level) : _logLevel(level) {}
		virtual ~Logger() {}

		template <class T>
		Logger& operator<<(T&& rhs)
		{
			switch (_logLevel)
			{
			case INFO:
				std::cout << "[INFO] ";
				break;
			case ERR:
				std::cout << "[ERROR] ";
				break;
			case DEBUG:
				std::cout << "[DEBUG] ";
				break;
			default:
				break;
			} 
			std::cout << std::forward<T>(rhs);
			return _tail;
		}

		Logger& operator<<(std::ostream& (*os)(std::ostream&))
		{
			switch (_logLevel)
			{
			case DEBUG:
				std::cout << std::endl;
				break;
			default:
				std::cout << std::endl;
				break;
			}
			return _tail;
		}

	private:

	public:

	private:
		LogLevel	_logLevel;
		static Logger _tail;
	};
	static Logger cDebug{ Logger::LogLevel::DEBUG };
	static Logger cInfo{ Logger::LogLevel::INFO };
	static Logger cWarning{ Logger::LogLevel::WARNING };
	static Logger cError{ Logger::LogLevel::ERR };
	static Logger cFatal{ Logger::LogLevel::FATAL };
}