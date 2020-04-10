#pragma once

#include "EventSystem.hpp"
#include "EngineDefines.h"

#include <string>
#include <vector>

#define BAKERS_LOG_MESSAGE(str)	Core::Debug::Logger::AddLog(Core::Debug::ELogType::LOG_MESSAGE, str, __FILE__, __FUNCTION__, __LINE__)
#define BAKERS_LOG_WARNING(str)	Core::Debug::Logger::AddLog(Core::Debug::ELogType::LOG_WARNING, str, __FILE__, __FUNCTION__, __LINE__)
#define BAKERS_LOG_ERROR(str)	Core::Debug::Logger::AddLog(Core::Debug::ELogType::LOG_ERROR, str, __FILE__, __FUNCTION__, __LINE__)

namespace Core::Debug
{
	/**
	 * Log type
	 */
	enum class ELogType
	{
		LOG_MESSAGE = 0,
		LOG_WARNING,
		LOG_ERROR,

		COUNT
	};

	/**
	 * Log data
	 */
	struct LogData
	{
		/**
		 * Log type
		 */
		ELogType type;

		/**
		 * Log message
		 */
		const char* message;

		/**
		 * The time when the log was displayed 
		 */
		const char* time;

		/**
		 * File where the log is called
		 */
		const char* file;

		/**
		 * Function where the log is called
		 */
		const char* function;

		/**
		 * File line where the log is called
		 */
		int line;
	};

	/**
	 * Log handler
	 */
	BAKERS_API_CLASS Logger
	{
	public:
		/**
		 * Delete constructor
		 */
		Logger() = delete;

		/**
		 * Add log in container
		 * @param type: Type log
		 * @param messageLog: messageLog
		 * @param file: File where the log is called
		 * @param function: Function where the log is called
		 * @param line: File line where the log is called
		 */
		static void AddLog(ELogType type, std::string messageLog, const char* file, const char* function, int line) noexcept;

		/**
		 * Add an event
		 * @param call: Callback function
		 * @return The ID of created listener
		 */
		static Core::SystemManagement::ID AddEvent(const std::function<void(const LogData&)>& call) noexcept;

		/**
		 * Remove an event through its ID
		 * @param listenerID: ID of listener to remove
		 * @return Whether the deletion work or not
		 */
		static bool RemoveListener(Core::SystemManagement::ID listenerID) noexcept;

		/**
		 * Clear logs
		 */
		static void ClearLogs() noexcept;

		/**
		 * Get size of logs data
		 * @return Size of logs data
		 */
		static size_t GetLogsDataSize() noexcept;

		/**
		 * Get Logs data
		 * @return Logs data
		 */
		static LogData* GetLogsData() noexcept;

		/**
		 * Get the number of logs of the given type
		 * @param type: The log type
		 * @return The number of logs of this type
		 */
		static int GetNbLogs(ELogType type) noexcept;
	};

	inline std::string	ToString(const ELogType log) noexcept
	{
		switch (log)
		{
		case ELogType::LOG_MESSAGE:
			return "Messages";
		case ELogType::LOG_WARNING:
			return "Warnings";
		case ELogType::LOG_ERROR:
			return "Errors";
		default:
			return "Unknow log type";
		}
	}
};
