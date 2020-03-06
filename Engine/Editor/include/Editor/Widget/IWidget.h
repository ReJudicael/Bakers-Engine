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
	 * Interface component enabling features for the user
	 */
	namespace Widget
	{
		/**
		 * Interface component enabling features for the user
		 */
		class IWidget
		{
		public:
			/**
			 * Title of widget
			 */
			const char* m_name;

			/**
			 * Whether the widget window is visible
			 */
			bool m_isVisible{ true };

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
			 * Constructor which set title of the widget window
			 * @param name: Set title of window
			 */
			IWidget(const char* name);

			/**
			 * Default destructor
			 */
			~IWidget() = default;

		private:
			/**
			 * Begin widget window
			 * @return True if window has begun otherwise false
			 */
			bool Begin();

			/**
			 * End widget window
			 * @return True if window has ended otherwise false
			 */
			bool End();

		public:
			/**
			 * Display elements of widget in window
			 */
			virtual void Tick() = 0;

			/**
			 * Display window and widget
			 */
			void Draw();
		};
	}
}
