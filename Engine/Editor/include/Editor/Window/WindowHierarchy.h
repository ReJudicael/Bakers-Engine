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

		void PopupMenu(Core::Datastructure::Object* object);

	private:

		void ShowChildrenOfObject(Core::Datastructure::Object* object);

		/**
		 * Draw elements in window
		 */
		void Tick() override;
	};
}
