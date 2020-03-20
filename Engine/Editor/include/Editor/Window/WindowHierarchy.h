#pragma once

#include "AWindow.h"

namespace Core::Datastructure
{
	class Object;
}

namespace Editor::Window
{
	/**
	 * Widget Hierarchy
	 */
	class WindowHierarchy final : public AWindow
	{
	public:
		/**
		 * Constructor which set title of window ("Hierarchy")
		 */
		WindowHierarchy(Canvas* parent, bool visible = true);

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

		void ShowChildrenOfObject(Core::Datastructure::Object* object);

		/**
		 * Draw elements in window
		 */
		void Tick() override;
	};
}
