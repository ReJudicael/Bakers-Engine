#pragma once

#include "IWidget.h"

namespace Editor::Widget
{
	/**
	 * Widget Hierarchy
	 */
	class WidgetHierarchy final : public IWidget
	{
	public:
		/**
		 * Constructor which set title of window ("Hierarchy")
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
