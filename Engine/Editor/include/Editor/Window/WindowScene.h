#pragma once

#include "AWindow.h"

namespace Editor::Window
{
	/**
	 * Widget Scene
	 */
	class WindowScene final : public AWindow
	{
	public:
		/**
		 * Constructor which set title of window ("Scene")
		 */
		WindowScene(bool visible = true);

		/**
		 * Default destructor
		 */
		~WindowScene() = default;

		/**
		 * Draw elements of widget in window
		 */
		void Tick() override;
	};
}
