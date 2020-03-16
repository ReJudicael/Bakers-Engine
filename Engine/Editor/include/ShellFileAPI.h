#pragma once

#include <string>

namespace ShellFileAPI
{
	/**
	 * Open a specified file
	 * @param path: Path to wanted file
	 */
	void OpenSpecifiedFile(const std::string& pathFile) noexcept;
}
