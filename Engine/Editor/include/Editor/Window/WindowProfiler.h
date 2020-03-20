#pragma once

#include "AWindow.h"

namespace Editor::Window
{
	/**
	 * Widget Profiler
	 */
	class WindowProfiler : public AWindow
	{
	public:
		/**
		 * Constructor which set title of window ("Console")
		 */
		WindowProfiler(Canvas* parent, bool visible = true);

		/**
		 * Default destructor
		 */
		~WindowProfiler() = default;

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
