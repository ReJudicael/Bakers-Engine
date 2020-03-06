#pragma once

#include "IWidget.h"

namespace Editor::Widget
{
	/**
	 * Widget Scene
	 */
	class WidgetScene : public IWidget
	{
	public:
		/**
		 * Constructor which set title of window
		 */
		WidgetScene();

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
