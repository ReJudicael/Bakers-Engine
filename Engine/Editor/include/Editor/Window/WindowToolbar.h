#pragma once

#include "AWindow.h"
#include "Texture.h"

namespace Editor::Window
{
	/**
	 * Window Toolbar
	 */
	class WindowToolbar final : public AWindow
	{
	private:
		/**
		 * Icons in Toolbar: "Translate", "Rotate", "Scale"
		 */
		std::shared_ptr<Resources::Texture> m_guizmoIcons[5];

		/**
		 * Icons in Toolbar: "Play", "Stop", "Pause", "Next", "Compile"
		 */
		std::shared_ptr<Resources::Texture> m_simulationIcons[5];

	public:
		/**
		 * Constructor which set title of window ("Toolbar")
		 */
		WindowToolbar(Canvas* canvas, bool visible = true);

		/**
		 * Default destructor
		 */
		~WindowToolbar() = default;

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
		 * Display Toolbar
		 */
		void DisplayToolbar();

	private:
		/**
		 * Draw elements in window
		 */
		void Tick() override;
	};
}
