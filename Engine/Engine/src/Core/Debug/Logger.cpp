#include "Logger.h"
#include <time.h>

namespace Core::Debug
{
	std::vector<LogData> logData;

	static std::string GetTimeAsTring()
	{
		std::string _time;
		const time_t _now = time(nullptr);
		tm _tm;

		localtime_s(&_tm, &_now);

		std::string timeData[3] =
		{
			std::to_string(_tm.tm_hour),
			std::to_string(_tm.tm_min),
			std::to_string(_tm.tm_sec)
		};

		for (int i = 0; i < 3; ++i)
			if (timeData[i].size() < 2)
				timeData[i].insert(timeData[i].begin(), '0');

		for (int i = 0; i < 3; ++i)
		{
			_time += timeData[i];
			if (i < 2)
				_time += ':';
		}

		return _time;
	}

	void Logger::DebugLog(LogType type, std::string messageLog, const char* file, int line, const char* function) noexcept
	{
		if (messageLog.size() <= 0)
			return;

		const std::string _time = GetTimeAsTring();

		char* time = (char*)malloc(sizeof(char) * 9);
		memcpy(time, _time.c_str(), 9);

		char* message = (char*)malloc(sizeof(char) * (messageLog.size() + 1));
		memcpy(message, messageLog.c_str(), messageLog.size() + 1);

		logData.push_back({ type, line, message, time, file, function });
	}

	void Logger::ClearLogs()
	{
		logData.clear();
	}

	size_t	Logger::GetLogdataSize()
	{
		return logData.size();
	}

	LogData* Logger::GetLogdata()
	{
		return logData.data();
	}
}
