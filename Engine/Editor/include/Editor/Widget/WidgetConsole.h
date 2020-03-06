#pragma once
#include "IWidget.h"

namespace Editor::Widget
{
	/**
	 * Widget Console
	 */
	class WidgetConsole : public IWidget
	{
	public:
		/**
		 * Constructor which set title of window
		 * @param name: Title of window
		 */
		WidgetConsole(const char* name);

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
