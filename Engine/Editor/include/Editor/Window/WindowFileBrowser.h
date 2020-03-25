#pragma once

#include "AWindow.h"
#include "FileSystem.hpp"
#include <stdio.h>
#include <map>

#define PATH_TO_ICONS "Resources\\Images\\icon_"

namespace Editor::Window
{
	/**
	 * Widow File Browser
	 */
	class WindowFileBrowser final : public AWindow
	{
	private:
		/**
		 * FileSystem to handle path navigation
		 */
		Core::SystemManagement::FileSystem fs;

		/**
		 * Name of the path to be renamed
		 */
		std::string m_renamePath = "";

		/**
		 * Decide whether the user can rename an other path or not
		 */
		bool m_canRename{ false };

		/**
		 * Ratio size (1 to 100), to zoom path contents
		 */
		int m_size{ 1 };

		/**
		 * Size of path contents
		 */
		float m_contentPathSize{ 0.f };

	public:
		/**
		 * Constructor which set title of window ("File Browser")
		 */
		WindowFileBrowser(Canvas* canvas, bool visible = true);

		/**
		 * Default destructor
		 */
		~WindowFileBrowser() = default;

	private:
		/**
		 * Push window style
		 */
		void PushWindowStyle() override;

		/**
		 * Pop window style
		 */
		void PopWindowStyle() override;

	private:
		/**
		 * Rename the selected file / folder
		 * @param itemName: Name of the chosen file / folder
		 */
		void RenameContent(const std::string& itemName);

		/**
		 * Pop-up displayed when mouse right button is pressed
		 * @param itemPath: Path of the chosen file / folder
		 */
		void PopupMenu(const std::string& itemPath);

		/**
		 * Slider to zoom the displayed content
		 */
		void ZoomPathContents();

		/**
		 * Get the texture of the icon of the chosen file / folder
		 * @param itemPath: Path of the chosen file / folder
		 */
		ImTextureID GetIcon(const std::string& itemPath);

	private:
		/**
		 * Show current local path on header
		 */
		void ShowCurrentPathOnHeader();

		/**
		 * Whether the file has an excluded extension or not
		 * @param itemName: Name of the chosen file / folder
		 */
		bool FileHasExcludedExtension(const std::string& itemName);

		/**
		 * Show the chosen file / folder
		 * @param itemName: Name of the chosen file / folder
		 * @param itemPath: Path of the chosen file / folder
		 */
		void ShowItem(const std::string& itemName, const std::string& itemPath);

		/**
		 * Display the contents of the current directory
		 * @param contents: Contents of the current directory
		 */
		void ShowDirectoryContent(std::vector<std::filesystem::path> contents);

		/**
		 * Display the current local path and the contents of the current directory
		 * @param contents: Contents of the current directory
		 */
		void ShowFileBrowser(const std::vector<std::filesystem::path>& content);

	private:
		/**
		 * Draw elements in window
		 */
		void Tick() override;
	};
}
