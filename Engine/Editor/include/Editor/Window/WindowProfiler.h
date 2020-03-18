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
		WindowProfiler(EditorEngine* engine, bool visible = true);

		/**
		 * Default destructor
		 */
		~WindowProfiler() = default;

		/**
		 * Draw elements of widget in window
		 */
		void Tick() override;
	};
}
