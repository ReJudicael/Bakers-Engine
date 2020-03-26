#pragma once

#include "AWindow.h"

namespace Core::Datastructure
{
	class Object;
}

namespace Editor::Window
{
	/**
	 * Window Hierarchy
	 */
	class WindowHierarchy final : public AWindow
	{
	private:
		bool m_destroySelected = false;
		bool m_canRename = false;
		ImVec2 m_cursorPos;
		Core::Datastructure::Object* m_objectToRename{ nullptr };
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

		void RenameObject(Core::Datastructure::Object* object);

	private:
		/**
		 * Pop-up displayed when mouse right button is pressed on item
		 * @param object: Item/Object on which the right mouse button has been pressed
		 */
		void PopupMenuOnItem(Core::Datastructure::Object* object);

		/**
		 * Show children of object
		 * @param object: Object from which the children are shown/displayed.
		 */
		void ShowChildrenOfObject(Core::Datastructure::Object* object);

	private:
		/**
		 * Draw elements in window
		 */
		void Tick() override;
	};
}
