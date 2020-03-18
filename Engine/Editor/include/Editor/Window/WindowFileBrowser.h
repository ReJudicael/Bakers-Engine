#pragma once

#include "AWindow.h"
#include "FileSystem.hpp"
#include <stdio.h>
#include <map>

#define PATH_TO_ICONS "Resources\\Images\\icon_"

namespace Editor::Window
{
	/**
	 * Widget File Browser
	 */
	class WindowFileBrowser final : public AWindow
	{
	private:
		/**
		 * FileSystem to handle path navigation
		 */
		Core::SystemManagement::FileSystem fs;

		/**
		 * Map contains the name of icon and its texture ID
		 */
		std::map<std::string, ImTextureID> icons;

		/**
		 * Name of the path to be renamed
		 */
		std::string m_renamePath = "";

		/**
		 * Decide whether the user can rename an other path or not
		 */
		bool m_canRename{ false };

		/**
		 * Ratio size ( 1 to 100), to zoom path contents
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
		WindowFileBrowser(EditorEngine* engine, bool visible = true);

		/**
		 * Default destructor
		 */
		~WindowFileBrowser() = default;

	private:
		/**
		 * Push widget style
		 */
		void PushWidgetStyle();

		/**
		 * Pop widget style
		 */
		void PopWidgetStyle();

	private:
		/**
		 * Draw elements of widget in window
		 */
		void Tick() override;

	private:
		/**
		 * Show current local path
		 */
		void ShowCurrentLocalPath();

		void RenameContent(const std::string& itemPath, const std::string& item);
		void DirectoryContentActionRight(const std::string& itemPath);
		void ZoomPathContents();
		void ShowDirectoryContents(std::vector<std::filesystem::path> contents);
		void ShowDirectory(const std::vector<std::filesystem::path>& content);
		ImTextureID GetIcon(const std::string& item);
	};
}
