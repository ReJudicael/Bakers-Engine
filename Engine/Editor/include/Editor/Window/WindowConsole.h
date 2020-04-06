#pragma once

#include "AWindow.h"
#include "Texture.h"

namespace Editor::Window
{
	/**
	 * Window Console
	 */
	class WindowConsole final : public AWindow
	{
	private:
		/**
		 * Icons to display logs
		 */
		std::shared_ptr<Resources::Texture> m_icons[3];

		/**
		 * Wheter the icon is enabled or not
		 */
		bool m_iconIsEnabled[3]{ true, true, true };

		// Text Filter
		ImGuiTextFilter m_textFilter;

	public:
		/**
		 * Constructor which set title of window ("Console")
		 */
		WindowConsole(Canvas* canvas, bool visible = true);

		/**
		 * Default destructor
		 */
		~WindowConsole() = default;

	private:
		/**
		 * Push window style
		 */
		void PushWindowStyle() override;

		/**
		 * Pop window style
		 */
		void PopWindowStyle() override;


	private:
		/**
		 * Add log button
		 * @param icon: Icon  of button
		 * @param label: Label of button
		 * @param isEnabled: Whether the button is enabled or not
		 */
		void AddLogButton(const std::shared_ptr<Resources::Texture>& icon, const std::string& label, bool& isEnabled);

		/**
		 * Console header that contains the log buttons and the filter
		 */
		void ConsoleHeader();

		/**
		 * Draws the console messages
		 */
		void	ConsolePrint();

	private:
		/**
		 * Draw elements in window
		 */
		void Tick() override;
	};
}
