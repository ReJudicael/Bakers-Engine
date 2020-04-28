#pragma once

#include <string>
#include "CoreMinimal.h"

namespace Core::SystemManagement
{
	/**
	 * Shell file API, used by Windows Property System
	 */
	namespace ShellFileAPI
	{
		/**
		 * Open a specified file
		 * @param path: Path to wanted file
		 */
		BAKERS_API void OpenSpecifiedFile(const std::string& pathFile) noexcept;

		/**
		 * Open a specified link
		 * @param link: Link wanted
		 */
		BAKERS_API void OpenSpecifiedLink(const std::string& link) noexcept;
	}
}