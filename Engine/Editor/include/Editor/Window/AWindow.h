#pragma once

#include <imgui\imgui.h>
#include <string>

#include "DrawableUIComponent.h"

namespace Editor
{
	class Canvas;
	class EditorEngine;

	/**
	 * Window enabling features for the user
	 */
	namespace Window
	{
		/**
		 * Base class for windows
		 */
		class AWindow : public Datastructure::DrawableUIComponent
		{
		protected:
			/**
			 * Whether the widget window has begun
			 */
			bool m_windowBegun{ false };

			/**
			 * Window flags
			 */
			ImGuiWindowFlags m_flags{ ImGuiWindowFlags_NoCollapse };

		public:
			/**
			 * Whether the widget window is visible
			 */
			bool isVisible{ true };

		protected:
			/**
			 * Canvas of the window
			 */
			Canvas* m_canvas;

		public:
			/**
			 * Explicit constructor
			 * @param canvas: Canvas of window
			 * @param name: Set title of window
			 * @param visible: Set whether the window is visible or not
			 */
			AWindow(Canvas* canvas, const std::string& name, bool visible = true);

			/**
			 * Destructor
			 */
			virtual ~AWindow();

		private:
			/**
			 * Begin widget window
			 * @return True if window has begun otherwise false
			 */
			bool Begin();

			/**
			 * End widget window
			 */
			void End();

		public:
			/**
			 * Focus widget
			 */
			void Focus();

			/**
			 * Indicate the last frame the window was set to focused
			 * @return The last frame the window has been focused
			 */
			int LastFrameFocused();

		protected:
			/**
			 * Push window style
			 */
			virtual void PushWindowStyle() = 0;

			/**
			 * Pop window style
			 */
			virtual void PopWindowStyle() = 0;

			/**
			 * Display elements of widget in window
			 */
			virtual void Tick() = 0;

		public:
			/**
			 * Display window and widget
			 */
			void Draw() override;

		public:
			/**
			 * Returns the manager of the canvas
			 * @return The manager of the canvas
			 */
			Canvas* GetCanvas() noexcept;

			/**
			 * Returns the engine core
			 * @return The engine core
			 */
			EditorEngine* GetEngine() noexcept;
		};
	}
}
