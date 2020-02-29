#include "Log.h"

Core::Debug::Logger* Core::Debug::Logger::m_instance = nullptr;

void Core::Debug::Logger::AddLog(LogData data)
{
	m_logdata.push_back(data);
}

void Core::Debug::Logger::DebugLog(LogType type, const char* messageLog, const char* file, int line, const char* function) noexcept
{
	GetInstance()->AddLog({ type, messageLog, file, line, function });

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD wAtt;

	switch (type)
	{
	case LogType::LOG:
		wAtt = 0x0F;
		break;
	case LogType::LOG_WARNING:
		wAtt = 0x0E;
		break;
	case LogType::LOG_ERROR:
		wAtt = 0x0C;
		break;
	default:
		wAtt = 0x0F;
		break;
	}

	SetConsoleTextAttribute(hConsole, wAtt);
	std::cout << "[" << function << "] " << messageLog << std::endl;
	SetConsoleTextAttribute(hConsole, 0x0F);
}

Core::Debug::Logger* Core::Debug::Logger::GetInstance()
{
	if (!m_instance)
		m_instance = new Logger;
	return m_instance;
}
