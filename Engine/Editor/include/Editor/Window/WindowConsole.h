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
		WindowConsole(Canvas* parent, bool visible = true);

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
