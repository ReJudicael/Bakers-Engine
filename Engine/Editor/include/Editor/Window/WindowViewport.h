#pragma once

#include "AWindow.h"

namespace Core::Renderer
{
	class Framebuffer;
}

namespace Editor::Window
{
	/**
	 * Window Viewport
	 */
	class WindowViewport final : public AWindow
	{
	private:
		/**
		 * Current camera beeing displayed
		 */
		int	m_cameraNum{ 0 };

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
		 * Choose viewport
		 */
		void ChooseViewport();

		/**
		 * Display FBO
		 */
		void DisplayFBO(Core::Renderer::Framebuffer* fbo);

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
