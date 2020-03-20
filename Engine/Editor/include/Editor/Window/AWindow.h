#pragma once

#include "DrawableComponent.h"

#include <imgui\imgui.h>
#include <string>

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
		 * Base class for widgets
		 */
		class AWindow : public Datastructure::DrawableComponent
		{
		protected:
			/**
			 *  Whether the widget window has begun
			 */
			bool m_windowBegun{ false };

			/**
			 * Parent of the window
			 */
			Canvas* m_canvas;

		public:
			/**
			 * Returns the canvas containing the AWindow
			 */
			Canvas* GetCanvas() noexcept;

			/**
			 * Returns a pointer to the engine core
			 */
			EditorEngine* GetEngine() noexcept;

			/**
			 * Window flags
			 */
			ImGuiWindowFlags m_flags{ ImGuiWindowFlags_NoCollapse };

			/**
			 * Whether the widget window is visible
			 */
			bool isVisible{ true };

		public:
			/**
			 * Explicit constructor which set title of the widget window
			 * @param name: Set title of window
			 */
			AWindow(Canvas * parent, const std::string& name, bool visible = true);

			/**
			 * Destructor
			 */
			~AWindow();

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
			 * Focus widget
			 */
			void Focus();

			/**
			 * Display window and widget
			 */
			void Draw() override;
		};
	}
}
