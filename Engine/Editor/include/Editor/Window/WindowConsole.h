#pragma once

#include "AWindow.h"

namespace Editor::Window
{
	/**
	 * Widget Console
	 */
	class WindowConsole final : public AWindow
	{
	public:
		/**
		 * Constructor which set title of window ("Console")
		 */
		WindowConsole();

		/**
		 * Default destructor
		 */
		~WindowConsole() = default;

		/**
		 * Draw elements of widget in window
		 */
		void Tick() override;
	};
}
