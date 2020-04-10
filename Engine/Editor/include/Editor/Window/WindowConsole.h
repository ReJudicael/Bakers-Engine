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
		 * Icon for logs and button state, for each log type (Message, Warning, Error)
		 */
		std::pair<std::shared_ptr<Resources::Texture>, bool> m_logsIcon[3];

		/**
		 * Icon for settings
		 */
		std::shared_ptr<Resources::Texture> m_settingsIcon;

		/**
		 * Log Filter
		 */
		ImGuiTextFilter m_logFilter;

		/**
		 * Whether the scrollbar may be at the bottom or not
		 */
		bool m_canScrollBottom{ false };

		/**
		 * Wheter the autoScroll is activated or not
		 */
		bool m_autoScroll{ true };

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
		 * Callback called when adding a log
		 * @param log: Log being added
		 */
		void LogCallback(const Core::Debug::LogData& log);

		/**
		 * Add log button
		 * @param icon: Icon  of button
		 * @param label: Label of button
		 * @param isEnabled: Whether the button is enabled or not
		 */
		void AddLogButton(const std::shared_ptr<Resources::Texture>& icon, const std::string& label, const std::string& help_marker, bool& isEnabled);

		/**
		 * Settings button
		 */
		void SettingsButton();

		/**
		 * Console header that contains the log buttons, the settings button and the filter
		 */
		void ConsoleHeader();

		/**
		 * Print a log
		 * @param log: Log to print
		 */
		void PrintLog(Core::Debug::LogData log);

		/**
		 * Draws the console messages
		 */
		void ConsolePrint();

	private:
		/**
		 * Draw elements in window
		 */
		void Tick() override;
	};
}
