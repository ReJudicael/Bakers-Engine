#pragma once

#include <unordered_map>

#include "AWindow.h"
#include "FileSystem.hpp"
#include "Texture.h"

namespace Editor::Window
{
	/**
	 * Widow File Browser
	 */
	class WindowFileBrowser final : public AWindow
	{
	private:
		/**
		 * InputText flags to rename a path
		 */
		ImGuiInputTextFlags m_inputTextFlags;

		/**
		 * Path Filter
		 */
		ImGuiTextFilter m_pathFilter;

	private:
		/**
		 * FileSystem to handle path navigation
		 */
		Core::SystemManagement::FileSystem* m_fs;

		/**
		 * Name of the path to be renamed
		 */
		std::string m_renamePath{ "" };

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
		 * Destructor
		 */
		~WindowFileBrowser();

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

		/**
		 * Slider to zoom the displayed content
		 */
		void ZoomPathContents();

		/**
		 * Get the texture of the icon of the chosen file / folder
		 * @param itemPath: Path of the chosen file / folder
		 */
		GLuint GetIcon(const std::string& itemPath);

	private:
		/**
		 * Show current local path on header
		 */
		void ShowCurrentPathOnHeader();

		/**
		 * Drag an item
		 * @param itemName: Name of the dragged file / folder
		 * @param itemPath: Path of the dragged file / folder
		 */
		void DragDropSourceItem(const std::string& itemName, const std::string& itemPath);

		/**
		 * Drop an item in a folder
		 * @param itemPath: Path of the dropped file / folder
		 */
		void DragDropTargetItem(const std::string& itemPath);

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

	private:
		/**
		 * Draw elements in window
		 * Display the current local path and the contents of the current directory
		 */
		void Tick() override;
	};
}
