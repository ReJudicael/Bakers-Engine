#pragma once

#include "AWidget.h"

namespace Editor::Widget
{
	/**
	 * Widget Inspector
	 */
	class WidgetInspector final : public AWidget
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
