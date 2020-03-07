#pragma once

#include "IWidget.h"

namespace Editor::Widget
{
	/**
	 * Widget Inspector
	 */
	class WidgetInspector final : public IWidget
	{
	public:
		/**
		 * Constructor which set title of window ("Inspector")
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
