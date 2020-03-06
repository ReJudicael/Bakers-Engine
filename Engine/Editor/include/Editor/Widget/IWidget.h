#pragma once

#include <imgui\imgui.h>

#define W_SCENE "Scene"
#define W_HIERARCHY "Hierarchy"
#define W_INSPECTOR "Inspector"
#define W_FILEBROWSER "File Browser"
#define W_CONSOLE "Console"

namespace Editor
{
	/**
	 * A component of an interface that enables a user to perform a function
	 */
	namespace Widget
	{
		/**
		 * A component of an interface that enables a user to perform a function
		 */
		class IWidget
		{
		public:
			/**
			 * Title of widget
			 */
			const char* m_name;

			/**
			 * Window of widget is visible
			 */
			bool m_isVisible{ true };

			/**
			 * Window du widget has begun
			 */
			bool m_windowBegun{ false };

			/**
			 * Window flags
			 */
			ImGuiWindowFlags m_flags{ ImGuiWindowFlags_NoCollapse };

		public:
			/**
			 * Constructor which set title of window of widget
			 * @param name: Set title of window
			 */
			IWidget(const char* name);

			/**
			 * Default destructor
			 */
			~IWidget() = default;

		private:
			/**
			 * Begin window of widget
			 * @return True if window has begun otherwise false
			 */
			bool Begin();

			/**
			 * End window of widget
			 * @return True if window has ended otherwise false
			 */
			bool End();

		public:
			/**
			 * Draw elements of widget in window
			 */
			virtual void Tick() = 0;

			/**
			 * Draw window and widget
			 */
			void Draw();
		};
	}
}
