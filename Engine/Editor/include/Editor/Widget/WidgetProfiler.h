#pragma once

#include "AWidget.h"

namespace Editor::Widget
{
	/**
	 * Widget Profiler
	 */
	class WidgetProfiler : public AWidget
	{
	public:
		/**
		 * Constructor which set title of window ("Console")
		 */
		WidgetProfiler();

		/**
		 * Default destructor
		 */
		~WidgetProfiler() = default;

		/**
		 * Draw elements of widget in window
		 */
		void Tick() override;
	};
}
