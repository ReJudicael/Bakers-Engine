#pragma once

#include "IWidget.h"

namespace Editor::Widget
{
	/**
	 * Widget Inspector
	 */
	class WidgetInspector : public IWidget
	{
	public:
		/**
		 * Constructor which set title of window
		 */
		WidgetInspector();

		/**
		 * Default destructor
		 */
		~WidgetInspector() = default;

		/**
		 * Draw elements of widget in window
		 */
		void Tick() override;
	};
}
