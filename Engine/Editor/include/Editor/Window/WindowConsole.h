#pragma once

#include "AWindow.h"
#include "Texture.h"

namespace Editor::Window
{
	/**
	 * Structure for each type of log
	 * to handle the console
	 */
	struct ConsoleLogDataType
	{
		/**
		 * Icon to display the type of log
		 */
		std::shared_ptr<Resources::Texture> icon;

		/**
		 * Number of logs (by type)
		 */
		unsigned int nbLogs{ 0 };

		/**
		 * Wheter the icon is enabled or not
		 */
		bool isEnabled{ true };
	};

	/**
	 * Window Console
	 */
	class WindowConsole final : public AWindow
	{
	private:
		/**
		 * Contains each type of log (Message, Warning, Error)
		 */
		ConsoleLogDataType m_log[static_cast<int>(Core::Debug::LogType::COUNT)];

		/**
		 * Text Filter
		 */
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
		void AddLogButton(const std::shared_ptr<Resources::Texture>& icon, const std::string& label, const std::string& help_marker, bool& isEnabled);

		/**
		 * Console header that contains the log buttons and the filter
		 */
		void ConsoleHeader();

		void LogPrint(Core::Debug::LogData log);

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
