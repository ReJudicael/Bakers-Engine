#pragma once

#include "AWidget.h"

namespace Editor::Widget
{
	/**
	 * Widget Hierarchy
	 */
	class WidgetHierarchy final : public AWidget
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
