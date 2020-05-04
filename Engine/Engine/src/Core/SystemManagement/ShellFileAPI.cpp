#include "ShellFileAPI.h"

#include <Windows.h>

namespace Core::SystemManagement::ShellFileAPI
{
	void OpenSpecifiedFile(const std::string& pathFile) noexcept
	{
		ShellExecuteA(NULL, "open", pathFile.c_str(), NULL, NULL, SW_SHOWNORMAL);
	}

	void OpenSpecifiedLink(const std::string& link) noexcept
	{
		ShellExecuteA(NULL, NULL, link.c_str(), NULL, NULL, SW_SHOW);
	}
}
