#pragma once

#include <imgui\imgui.h>
#include <string>

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
		private:
			/**
			 * ID of widget (incremented when a widget has been instantiated)
			 */
			static unsigned int _ID_WIDGET_INCREMENT;

		protected:
			/**
			 * Title of widget
			 */
			std::string m_name;

			/**
			 * Whether the widget window is visible
			 */
			bool m_isVisible{ true };

			/**
			 * Window flags
			 */
			ImGuiWindowFlags m_flags{ ImGuiWindowFlags_NoCollapse };

		public:
			/**
			 * Constructor which set title of the widget window
			 * @param name: Set title of window
			 */
			IWidget(std::string name);

			/**
			 * Default destructor
			 */
			~IWidget() = default;

			/**
			 * Get title of widget
			 * @return Title of widget
			 */
			std::string GetName() const;

			/**
			 * Get whether the widget window is visible
			 * @return True if the widget window is visible, false otherwise
			 */
			bool IsVisible() const;

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
