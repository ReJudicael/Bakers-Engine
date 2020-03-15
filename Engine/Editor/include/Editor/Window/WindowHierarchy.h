#pragma once

#include "AWindow.h"

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
		WindowHierarchy();

		/**
		 * Default destructor
		 */
		~WindowHierarchy() = default;

		/**
		 * Draw elements of widget in window
		 */
		void Tick() override;
	};
}
