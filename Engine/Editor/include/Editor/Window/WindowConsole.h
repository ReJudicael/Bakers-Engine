#pragma once

#include "AWindow.h"

namespace Editor::Window
{
	/**
	 * Window Console
	 */
	class WindowConsole final : public AWindow
	{
	public:
		/**
		 * Constructor which set title of window ("Console")
		 */
		WindowConsole(Canvas* canvas, bool visible = true);

		/**
		 * Default destructor
		 */
		~WindowConsole() = default;

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
		 * Draw elements in window
		 */
		void Tick() override;
	};
}
