#pragma once

#include <vector>
#include <memory>

#include "DrawableUIComponent.h"
#include "Container.hpp"
#include "AWidget.h"

namespace Editor
{
	/**
	 * Base class for menu bar
	 */
	class MenuBar final : public virtual Datastructure::IDrawable, public Datastructure::Container<Widget::AWidget>
	{
	private:
		/**
		 *  Whether the menu bar has begun
		 */
		bool m_menuBarBegun{ false };

	public:
		/**
		 * Default constructor
		 */
		MenuBar() = default;

		/**
		 * Destructor
		 */
		~MenuBar();

	private:
		/**
		 * Begin menu bar
		 * @return True if window has begun otherwise false
		 */
		bool Begin();

		/**
		 * End menu bar
		 */
		void End();

	public:
		/**
		 * Display window and widget
		 */
		void Draw() override;
	};
}
