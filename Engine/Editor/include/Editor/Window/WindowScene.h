#pragma once

#include "AWindow.h"

namespace Editor::Window
{
	/**
	 * Widget Scene
	 */
	class WindowViewport final : public AWindow
	{
	public:
		/**
		 * Constructor which set title of window ("Scene")
		 */
		WindowViewport(EditorEngine* engine, bool visible = true);

		/**
		 * Default destructor
		 */
		~WindowViewport() = default;

		/**
		 * Draw elements of widget in window
		 */
		void Tick() override;
	};
}
