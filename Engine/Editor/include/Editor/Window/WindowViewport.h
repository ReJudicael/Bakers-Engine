#pragma once

#include "AWindow.h"

namespace Editor::Window
{
	/**
	 * Window Viewport
	 */
	class WindowViewport final : public AWindow
	{
	public:
		/**
		 * Constructor which set title of window ("Viewport")
		 */
		WindowViewport(Canvas* canvas, bool visible = true);

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
		 * Display Viewport
		 */
		void DisplayViewport();

	private:
		/**
		 * Draw elements in window
		 */
		void Tick() override;
	};
}
