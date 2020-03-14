#pragma once

#include "AMenu.h"
#include "MenuItem.h"

namespace Editor::Menu
{
	/**
	 * Menu group contains menu groups and menu items
	 */
	class MenuGroup final : public AMenu
	{
	private:
		/**
		 * Menus (menu groups and menu items)
		 */
		std::vector<std::unique_ptr<AMenu>> m_menus;

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
		 * Add menu
		 * @tparam T: Specific menu class
		 * @tparam args: Arguments of constructor of given menu class
		 * @return Instance of menu
		 */
		template<class T, class ...Args>
		T& AddMenu(Args&& ...args);

		/**
		 * Remove menu
		 * @param menu: Menu to removes
		 * @warning Don't call it when menu is drawing
		 */
		void RemoveMenu(const AMenu& menu);

		/**
		 * Remove all menus
		 * @warning Don't call it when menu is drawing
		 */
		void RemoveAllMenus();

	public:
		/**
		 * Display menus in this menu group
		 */
		void Draw() override;
	};

	template<class T, class ...Args>
	inline T& MenuGroup::AddMenu(Args&& ...args)
	{
		static_assert(std::is_base_of<AMenu, T>::value);

		std::unique_ptr<T> menu = std::make_unique<T>(std::forward<Args>(args)...);
		T& output = *menu;
		m_menus.emplace_back(std::move(menu));
		return output;
	}
}
