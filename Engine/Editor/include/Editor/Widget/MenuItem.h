#pragma once

#include "AWidget.h"
#include "EventSystem.hpp"

namespace Editor::Widget
{
	/**
	 * Item of a menu
	 */
	class MenuItem final : public AWidget
	{
	private:
		/**
		 * Shortcut
		 */
		std::string m_shortcut;

		/**
		 * Whether item is selected or not
		 */
		bool* m_selected;

	public:
		/**
		 * Wheter menu is enabled or not
		 */
		bool isEnabled;

	public:
		/**
		 * Action launched when item is clicked
		 */
		Core::SystemManagement::EventSystem<> OnClick;

	public:
		/**
		 * Specific constructor
		 * @param name: Name of item
		 * @param shortcut: Shortcut displayed
		 * @param selected: Whether item is selected or not
		 * @param enabled: Whether item is enabled or not
		 */
		MenuItem(const std::string& name, const std::string& shortcut = "", bool* selected = nullptr, bool enabled = true);

		/**
		 * Destructor default
		 */
		~MenuItem() = default;

	private:
		/**
		 * Create menu item
		 */
		bool Begin();

	public:
		/**
		 * Display and Invoke event if clicked
		 */
		void Draw() override;
	};
}
