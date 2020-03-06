#pragma once

#include "IWidget.h"

namespace Editor::Widget
{
	/**
	 * Widget Hierarchy
	 */
	class WidgetHierarchy : public IWidget
	{
	public:
		/**
		 * Constructor which set title of window
		 */
		WidgetHierarchy();

		/**
		 * Default destructor
		 */
		~WidgetHierarchy() = default;

		/**
		 * Draw elements of widget in window
		 */
		void Tick() override;
	};
}
