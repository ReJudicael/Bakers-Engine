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
			 * Name of widget (Widget Name)
			 */
			std::string m_name;

			/**
			 * ID of widget (##0)
			 */
			std::string m_ID;

		public:
			/**
			 * Whether the widget window is visible
			 */
			bool isVisible{ true };

		private:
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
			IWidget(std::string name);

			/**
			 * Default destructor
			 */
			~IWidget();

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
			 * Get ID of widget
			 * @return ID of widget
			 */
			std::string GetID() const;

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
