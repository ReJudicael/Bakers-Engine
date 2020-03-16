#pragma once

#include "AWidget.h"
#include "MenuItem.h"
#include "Container.hpp"

namespace Editor::Widget
{
	/**
	 * Menu group contains menu groups and menu items
	 */
	class MenuGroup final : public AWidget, public Datastructure::Container<AWidget>
	{
	private:
		/**
		 *  Whether the menu group has begun
		 */
		bool m_menuGroupBegun{ false };

	public:
		/**
		 * Wheter menu is enabled or not
		 */
		bool isEnabled;

	public:
		/**
		 * Default constructor
		 */
		MenuGroup(const std::string& name, bool enabled = true);

		/**
		 * Destructor
		 */
		~MenuGroup();

	private:
		/**
		 * Begin menu group
		 * @return True if menu group has begun otherwise false
		 */
		bool Begin();

		/**
		 * End MenuGroup
		 */
		void End();

	public:
		/**
		 * Display menus in this menu group
		 */
		void Draw() override;
	};
}
