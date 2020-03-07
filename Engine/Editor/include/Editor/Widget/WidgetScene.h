#pragma once

#include "IWidget.h"

namespace Editor::Widget
{
	/**
	 * Widget Scene
	 */
	class WidgetScene final : public IWidget
	{
	public:
		/**
		 * Constructor which set title of window
		 * @param name: Title of window
		 */
		WidgetScene(const char* name);

		/**
		 * Default destructor
		 */
		~WidgetScene() = default;

		/**
		 * Draw elements of widget in window
		 */
		void Tick() override;
	};
}
