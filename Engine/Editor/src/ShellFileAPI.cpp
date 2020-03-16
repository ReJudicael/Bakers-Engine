#include "ShellFileAPI.h"

#include <Windows.h>

namespace ShellFileAPI
{
	void OpenSpecifiedFile(const std::string& pathFile) noexcept
	{
		ShellExecuteA(NULL, "open", (LPCSTR)pathFile.c_str(), NULL, NULL, SW_SHOWNORMAL);
	}
}
