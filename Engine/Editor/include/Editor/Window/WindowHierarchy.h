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
		 * Root object
		 */
		Core::Datastructure::RootObject* m_rootObject{ nullptr };

		/**
		 * Game object to rename
		 */
		Core::Datastructure::Object* m_objectToRename{ nullptr };

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
		 * Rename game object
		 * @param object: Item/Object on which the right mouse button has been pressed
		 */
		void RenameObject(Core::Datastructure::Object* object);

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
		 * Show children of object
		 * @param parent: Object from which the children are displayed
		 */
		void ShowChildrenOfObject(Core::Datastructure::Object* parent);

	private:
		/**
		 * Draw elements in window
		 */
		void Tick() override;
	};
}
