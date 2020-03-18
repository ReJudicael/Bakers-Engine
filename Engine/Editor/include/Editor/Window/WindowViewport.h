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

	private:
		/**
		 * Push window style
		 */
		void PushWindowStyle() override;

		/**
		 * Pop window style
		 */
		void PopWindowStyle() override;

	private:
		/**
		 * Draw elements in window
		 */
		void Tick() override;
	};
}
