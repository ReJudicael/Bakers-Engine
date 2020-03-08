#pragma once
#include "IWidget.h"

namespace Editor::Widget
{
	/**
	 * Widget Console
	 */
	class WidgetConsole final : public IWidget
	{
	public:
		/**
		 * Constructor which set title of window ("Console")
		 */
		WidgetConsole();

		/**
		 * Default destructor
		 */
		~WidgetConsole() = default;

		/**
		 * Draw elements of widget in window
		 */
		void Tick() override;
	};
}
