#pragma once

#include "DrawableComponent.h"
#include "AMenu.h"

#include <vector>
#include <memory>

namespace Editor::Menu
{
	/**
	 * Base class for menu bar
	 */
	class MenuBar final : public Datastructure::IDrawable
	{
	private:
		/**
		 * Menus in MenuBar
		 */
		std::vector<std::unique_ptr<AMenu>> m_menus;

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

	public:
		/**
		 * Add menu
		 * @tparam T: Specific Menu class
		 * @tparam args: Arguments of constructor of given Menu class
		 * @return Instance of menu
		 */
		template<class T, class ...Args>
		T& AddMenu(Args&&... args);

		/**
		 * Remove menu
		 * @param menu: Menu to remove
		 * @warning Don't call it when menu is drawing
		 */
		void RemoveMenu(const AMenu& menu);

		/**
		 * Remove all menu
		 * @warning Don't call it when menu is drawing
		 */
		void RemoveAllMenus();

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

	template<class T, class ...Args>
	inline T& MenuBar::AddMenu(Args&& ...args)
	{
		static_assert(std::is_base_of<AMenu, T>::value);

		std::unique_ptr<T> menu = std::make_unique<T>(std::forward<Args>(args)...);
		T& output = *menu;
		m_menus.emplace_back(std::move(menu));
		return output;
	}
}
