#pragma once

#include "AWidget.h"

namespace Editor::Widget
{
	/**
	 * Widget Scene
	 */
	class WidgetScene final : public AWidget
	{
	public:
		/**
		 * Constructor which set title of window ("Scene")
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
