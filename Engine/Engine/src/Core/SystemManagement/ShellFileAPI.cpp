#include "ShellFileAPI.h"

#include <Windows.h>

namespace Core::SystemManagement::ShellFileAPI
{
	void OpenSpecifiedFile(const std::string& pathFile) noexcept
	{
		ShellExecuteA(NULL, "open", pathFile.c_str(), NULL, NULL, SW_SHOWNORMAL);
	}
}
