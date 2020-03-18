#pragma once

#include "IRenderable.h"
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
		class IWidget : public IDrawable
		{
		private:
			/**
			 * ID of widget (incremented when a widget has been instantiated)
			 */
			static unsigned int _ID_WIDGET_INCREMENT;

		protected:
			/**
			 * Name ID of widget (Widget Name##0)
			 */
			std::string m_nameID;

			/**
			 * Name of widget without ID (Widget Name)
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
			 * Get name ID of widget
			 * @return Name ID of widget
			 */
			std::string GetNameID() const;

			/**
			 * Get name of widget
			 * @return Name of widget
			 */
			std::string GetName() const;

			/**
			 * Get whether the widget window is visible
			 * @return True if the widget window is visible, false otherwise
			 */
			bool IsVisible() const;

			/**
			 * Set visiblity of the widget window
			 * @param state: New widget window visibility state
			 */
			void SetVisible(bool state = true);

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
			 * Display window and widget
			 */
			void Draw() override;
		};
	}
}
