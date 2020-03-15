#pragma once

#include "DrawableComponent.h"

#include <imgui\imgui.h>
#include <string>

namespace Editor
{
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
			 * Window flags
			 */
			ImGuiWindowFlags m_flags{ ImGuiWindowFlags_NoCollapse };

		public:
			/**
			 * Whether the widget window is visible
			 */
			bool isVisible{ true };

		public:
			/**
			 * Explicit constructor which set title of the widget window
			 * @param name: Set title of window
			 */
			AWindow(const std::string& name);

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
