#pragma once

#include "AWindow.h"

namespace Core::Datastructure
{
	class Object;
	class RootObject;
}

namespace Editor::Window
{
	/**
	 * Window Hierarchy
	 */
	class WindowHierarchy final : public AWindow
	{
	private:
		/**
		 * TreeNode flags
		 */
		ImGuiTreeNodeFlags m_treeNodeFlags;

		/**
		 * InputText flags to rename an object
		 */
		ImGuiInputTextFlags m_inputTextFlags;

	private:
		/**
		 * Game object to rename
		 */
		Core::Datastructure::Object* m_objectToRename{ nullptr };

		/**
		 * Buffer InputText (to rename an item)
		 */
		char m_name[64]{};

		/**
		 * Decide whether the user can rename a game object or not
		 */
		bool m_canRename{ false };

		/**
		 * Whether scrolling has been set or not
		 */
		bool m_scrollSetted{ false };

		/**
		 * Whether the game object is destroyed or not
		 */
		bool m_destroyObject{ false };

	public:
		/**
		 * Constructor which set title of window ("Hierarchy")
		 */
		WindowHierarchy(Canvas* canvas, bool visible = true);

		/**
		 * Default destructor
		 */
		~WindowHierarchy() = default;

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
		 * Set name to given object
		 * @param object: Object to rename
		 */
		void ApplyNameToObject(Core::Datastructure::Object* object);

		/**
		 * Rename game object
		 * @param object: Item/Object on which the right mouse button has been pressed
		 * @param cursorPos: Position of InputText
		 */
		void RenameObject(Core::Datastructure::Object* object, const ImVec2& cursorPos);

		/**
		 * Create game object
		 * @param parent: Parent of the future game object
		 */
		void MenuItemCreate(Core::Datastructure::Object* parent);

		/**
		 * Pop-up displayed when mouse right button is pressed on window
		 * @param root: Root of game objects
		 */
		void PopupMenuOnWindow(Core::Datastructure::Object* root);

		/**
		 * Pop-up displayed when mouse right button is pressed on item
		 * @param object: Game object chosen
		 */
		void PopupMenuOnItem(Core::Datastructure::Object* object);

		/**
		 * Draw TreeNode of object
		 * @param object: Game object chosen
		 * @return True if the TreeNode is open, false otherwise
		 */
		bool DrawTreeNodeOfObject(Core::Datastructure::Object* object);

		/**
		 * Drag an item
		 * @param object: Dragged object
		 */
		void DragDropSourceItem(Core::Datastructure::Object * object);

		/**
		 * Drop an item
		 * @param object: Dropped object
		 */
		void DragDropTargetItem(Core::Datastructure::Object * object);

		/**
		 * Show children of object
		 * @param parent: Object from which the children are displayed
		 */
		void ShowChildrenOfObject(Core::Datastructure::Object* parent);

		/**
		 * Display scene in Hierarchy
		 * @param scene: Scene to display
		 */
		void DisplaySceneHierarchy(Core::Datastructure::RootObject* scene);

	private:
		/**
		 * Draw elements in window
		 */
		void Tick() override;
	};
}
