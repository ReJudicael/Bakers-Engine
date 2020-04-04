#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "EngineDefines.h"

namespace Core::Debug
{
	enum class LogType
	{
		LOG = 0,
		LOG_WARNING,
		LOG_ERROR
	};

	struct LogData
	{
		LogType type;
		const char* message;
		const char* file;
		int line;
		const char* function;
	};

	BAKERS_API_CLASS Logger
	{
	public:
		Logger() = delete;
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;

		static void DebugLog(LogType type, const char* messageLog, const char* file, int line, const char* function) noexcept;
	};
};

#define DEBUG_LOG(str) Core::Debug::Logger::DebugLog(Core::Debug::LogType::LOG, str, __FILE__, __LINE__, __FUNCTION__)
#define DEBUG_LOG_WARNING(str) Core::Debug::Logger::DebugLog(Core::Debug::LogType::LOG_WARNING, str, __FILE__, __LINE__, __FUNCTION__)
#define DEBUG_LOG_ERROR(str) Core::Debug::Logger::DebugLog(Core::Debug::LogType::LOG_ERROR, str, __FILE__, __LINE__, __FUNCTION__)
