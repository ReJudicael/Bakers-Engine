#pragma once

#include "AWindow.h"

namespace Editor::Window
{
	/**
	 * Widget Inspector
	 */
	class WindowInspector final : public AWindow
	{
	public:
		/**
		 * Constructor which set title of window ("Inspector")
		 */
		WindowInspector();

		/**
		 * Default destructor
		 */
		~WindowInspector() = default;

		/**
		 * Draw elements of widget in window
		 */
		void Tick() override;
	};
}
