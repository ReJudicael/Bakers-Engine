#pragma once

#include "AWindow.h"

namespace Editor::Window
{
	/**
	 * Widget Inspector
	 */
	class WindowInspector final : public AWindow
	{
	public:
		/**
		 * Constructor which set title of window ("Inspector")
		 */
		WindowInspector(EditorEngine* engine, bool visible = true);

		/**
		 * Default destructor
		 */
		~WindowInspector() = default;

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
