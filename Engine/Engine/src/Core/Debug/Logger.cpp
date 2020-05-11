#include "Logger.h"

#include <iostream>
#include <time.h>
#include <Windows.h>

namespace Core::Debug
{
	std::vector<LogData> logs;
	Core::SystemManagement::EventSystem<const LogData&> OnLogAdding;
	int nbLogs[3]{ 0 };

	static std::string GetCurrentTimeAsTring()
	{
		const time_t _now = time(nullptr);
		tm _tm;
		localtime_s(&_tm, &_now);

		std::string timeData[3] =
		{
			std::to_string(_tm.tm_hour),
			std::to_string(_tm.tm_min),
			std::to_string(_tm.tm_sec)
		};

		for (int i{ 0 }; i < 3; ++i)
			if (timeData[i].size() < 2)
				timeData[i].insert(timeData[i].begin(), '0');

		std::string _time;
		for (int i{ 0 }; i < 3; ++i)
		{
			_time += timeData[i];
			if (i < 2)
				_time += ':';
		}

		return _time;
	}

	static void DisplayLogInConsole(ELogType type, const char* function, const std::string& messageLog)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		WORD wAtt{ 0x0F };

		switch (type)
		{
		case ELogType::LOG_MESSAGE:	wAtt = 0x0F; break;
		case ELogType::LOG_WARNING:	wAtt = 0x0E; break;
		case ELogType::LOG_ERROR:	wAtt = 0x0C; break;
		}

		SetConsoleTextAttribute(hConsole, wAtt);
		std::cout << "[" << function << "] " << messageLog << std::endl;
		SetConsoleTextAttribute(hConsole, 0x0F);
	}

	void Logger::ClearLogs() noexcept
	{
		nbLogs[0] = nbLogs[1] = nbLogs[2] = 0;
		for (auto it : logs)
		{
			delete it.time;
			delete it.message;
		}
		logs.clear();
	}

	void Logger::AddLog(ELogType type, const std::string& messageLog, const char* file, const char* function, int line) noexcept
	{
		if (messageLog.size() <= 0)
			return;

		DisplayLogInConsole(type, function, messageLog);

		const std::string _time{ GetCurrentTimeAsTring() };
		char* time = (char*)malloc(sizeof(char) * 9);
#pragma warning(suppress : 6387)
		memcpy(time, _time.c_str(), 9);

		char* message = (char*)malloc(sizeof(char) * (messageLog.size() + 1));
#pragma warning(suppress : 6387)
		memcpy(message, messageLog.c_str(), messageLog.size() + 1);

		LogData log_data{ type, message, time, file, function, line };
		logs.push_back(log_data);
		OnLogAdding(log_data);

		++nbLogs[static_cast<int>(type)];
	}

	Core::SystemManagement::ID Logger::AddEvent(const std::function<void(const LogData&)>& call) noexcept
	{
		return OnLogAdding += call;
	}

	bool Logger::RemoveEvent(Core::SystemManagement::ID listenerID) noexcept
	{
		return OnLogAdding -= listenerID;
	}

	size_t	Logger::GetLogsDataSize() noexcept
	{
		return logs.size();
	}

	LogData* Logger::GetLogsData() noexcept
	{
		return logs.data();
	}

	int Logger::GetNbLogs(ELogType type) noexcept
	{
		switch (type)
		{
		case ELogType::LOG_MESSAGE: return nbLogs[0];
		case ELogType::LOG_WARNING: return nbLogs[1];
		case ELogType::LOG_ERROR: return nbLogs[2];
		}

		return 0;
	}
}
