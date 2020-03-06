#pragma once

#include "IWidget.h"
#include <stdio.h>

namespace Editor::Widget
{
	/**
	 * Widget File Browser
	 */
	class WidgetFileBrowser : public IWidget
	{
	public:
		/**
		 * Constructor which set title of window
		 */
		WidgetFileBrowser();

		/**
		 * Default destructor
		 */
		~WidgetFileBrowser() = default;

		/**
		 * Draw elements of widget in window
		 */
		void Tick() override;
	};
}
