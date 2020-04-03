#pragma once

#include "AWindow.h"
#include "FileSystem.hpp"
#include "Texture.h"
#include <unordered_map>

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
		std::string m_renamePath{ "" };

		/**
		 * InputText flags to rename a path
		 */
		ImGuiInputTextFlags m_inputTextFlags;

		/**
		 * Buffer InputText (to rename an item)
		 */
		char m_name[64]{};

		/**
		 * Decide whether the user can rename a path or not
		 */
		bool m_canRename{ false };

		/**
		 * Whether scrolling has been set or not
		 */
		bool m_scrollSetted{ false };

		/**
		 * Ratio size (1 to 100), to zoom path contents
		 */
		int m_size{ 1 };

		/**
		 * Size of path contents
		 */
		float m_contentPathSize{ 66.3f };

		/**
		 * Icons of the extensions
		 */
		std::unordered_map<std::string, std::shared_ptr<Resources::Texture>> m_icons;

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
		 * Whether the user can write in the InputText or not
		 * @return True if the user can write in the InputText, false otherwise
		 */
		bool CanRename();

		/**
		 * Set name to given item
		 * @param itemName: Name of the item to rename
		 */
		void ApplyNameToItem(const std::string& itemName);

		/**
		 * Rename the selected file / folder
		 * @param itemName: Name of the chosen file / folder
		 */
		void RenameContent(const std::string& itemName);

		/**
		 * Refresh contents in current path
		 */
		void MenuItemRefresh();

		/**
		 * Menu item view
		 */
		void MenuItemNew();

		/**
		 * Pop-up displayed when mouse right button is pressed on window
		 */
		void PopupMenuOnWindow();

		/**
		 * Pop-up displayed when mouse right button is pressed on item
		 * @param itemPath: Path of the chosen file / folder
		 */
		void PopupMenuOnItem(const std::string& itemPath);

	private:
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
		 * Show the chosen file / folder
		 * @param itemName: Name of the chosen file / folder
		 */
		void ShowItem(const std::string& itemName);

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
