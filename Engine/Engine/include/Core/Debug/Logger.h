#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "EngineDefines.h"

namespace Core::Debug
{
	enum class LogType
	{
		LOG_MESSAGE = 0,
		LOG_WARNING,
		LOG_ERROR,

		COUNT
	};

	struct LogData
	{
		LogType type;
		int line;
		const char* message;
		const char* time;
		const char* file;
		const char* function;
	};

	BAKERS_API_CLASS Logger
	{
	public:
		Logger() = delete;

		static void		DebugLog(LogType type, std::string messageLog, const char* file, int line, const char* function) noexcept;
		static void		ClearLogs();

		static size_t	GetLogdataSize();
		static LogData*	GetLogdata();
	};

	inline std::string ToString(const LogType log) noexcept
	{
		switch (log)
		{
		case LogType::LOG_MESSAGE:
			return "Messages";
		case LogType::LOG_WARNING:
			return "Warnings";
		case LogType::LOG_ERROR:
			return "Errors";
		default:
			return "Unknow log type";
		}
	}
};

#define DEBUG_LOG(str) Core::Debug::Logger::DebugLog(Core::Debug::LogType::LOG_MESSAGE, str, __FILE__, __LINE__, __FUNCTION__)
#define DEBUG_LOG_WARNING(str) Core::Debug::Logger::DebugLog(Core::Debug::LogType::LOG_WARNING, str, __FILE__, __LINE__, __FUNCTION__)
#define DEBUG_LOG_ERROR(str) Core::Debug::Logger::DebugLog(Core::Debug::LogType::LOG_ERROR, str, __FILE__, __LINE__, __FUNCTION__)
