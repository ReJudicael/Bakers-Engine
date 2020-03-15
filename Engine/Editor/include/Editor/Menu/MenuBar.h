#pragma once

#include "DrawableComponent.h"
#include "AMenu.h"
#include "Container.hpp"
#include <vector>
#include <memory>
#include <MenuGroup.h>

namespace Editor::Menu
{
	/**
	 * Base class for menu bar
	 */
	class MenuBar final : public Datastructure::IDrawable, public Datastructure::Container<Menu::MenuGroup>
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
