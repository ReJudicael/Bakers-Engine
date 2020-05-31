#include "WindowConsole.h"
#include "EditorEngine.h"

#include "IconsFontAwesome5.h"

namespace Editor::Window
{
	WindowConsole::WindowConsole(Canvas* canvas, bool visible) :
		AWindow{ canvas, ICON_FA_TERMINAL "  Console", visible }
	{
		for (auto& [icon, isEnabled] : m_logsIcon)
			isEnabled = true;

		m_tableFlags =	ImGuiTableFlags_ScrollY		| ImGuiTableFlags_ScrollFreezeTopRow	| ImGuiTableFlags_RowBg		|
						ImGuiTableFlags_Borders		| ImGuiTableFlags_BordersVFullHeight	| ImGuiTableFlags_Resizable	|
						ImGuiTableFlags_Reorderable	| ImGuiTableFlags_Hideable;

		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ConsoleIcons\\message.png", m_logsIcon[0].first);
		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ConsoleIcons\\warning.png", m_logsIcon[1].first);
		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ConsoleIcons\\error.png", m_logsIcon[2].first);

		m_listenerId = Core::Debug::Logger::AddEvent(BIND_EVENT(WindowConsole::LogCallback));
	}

	WindowConsole::~WindowConsole()
	{
		Core::Debug::Logger::RemoveEvent(m_listenerId);
	}

	void WindowConsole::PushWindowStyle()
	{
	}

	void WindowConsole::PopWindowStyle()
	{
	}

	void WindowConsole::LogCallback(const Core::Debug::LogData& log_data)
	{
		if (m_autoScroll)
			m_canScrollBottom = true;
	}

	void WindowConsole::LogButton(const std::shared_ptr<Resources::Texture>& icon, const std::string& label, const std::string& help_marker, bool& isEnabled)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, isEnabled ?
			ImGui::GetStyle().Colors[ImGuiCol_Button] :
			ImGui::GetStyle().Colors[ImGuiCol_FrameBg]);

		if (ImGui::ImageButtonUVWithText(icon->texture, label.c_str()))
			isEnabled = !isEnabled;
		ImGui::HelpMarkerItem(help_marker.c_str());

		ImGui::PopStyleColor();
	}

	void WindowConsole::ClearButton()
	{
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.91f, 0.25f, 0.09f, 0.8f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.91f, 0.25f, 0.09f, 0.5f });

		if (ImGui::Button("Clear", { 50.f, 0.f }))
			Core::Debug::Logger::ClearLogs();

		ImGui::PopStyleColor(2);
	}

	void WindowConsole::SettingsButton()
	{
		ImGui::PushStyleColor(ImGuiCol_Button, GImGui->Style.Colors[ImGuiCol_TableHeaderBg]);
		bool isClicked = ImGui::BeginComboButton(ICON_FA_COG);
		ImGui::HelpMarkerItem("Settings");
		ImGui::PopStyleColor();

		if (isClicked)
		{
			ImGui::MenuItem("Auto-scroll", (const char*)0, &m_autoScroll);
			ImGui::MenuItem("Clear on Play", (const char*)0, &m_clearOnPlay);
			ImGui::EndCombo();
		}
	}

	void WindowConsole::ConsoleHeader()
	{
		ClearButton();

		for (int i{ 2 }; i >= 0; --i)
		{
			const std::string& typeLog	= Core::Debug::ToString(static_cast<Core::Debug::ELogType>(i));
			const std::string& nbLogs	= std::to_string(Core::Debug::Logger::GetNbLogs(static_cast<Core::Debug::ELogType>(i)));
			const std::string& label	= m_logsIcon[i].second ? nbLogs : "0 of " + nbLogs;

			ImGui::SameLine();
			ImGui::PushID(i);
			LogButton(m_logsIcon[i].first, label, typeLog, m_logsIcon[i].second);
			ImGui::PopID();
		}
		ImGui::SameLine();
		SettingsButton();

		ImGui::SameLine(ImGui::GetContentRegionAvail().x - 175.f);
		m_logFilter.Draw("Filter", ImGui::GetContentRegionAvail().x - 37.f);
	}

	void WindowConsole::PrintLog(const Core::Debug::LogData& log)
	{
		if (m_logsIcon[static_cast<size_t>(log.type)].second && m_logFilter.PassFilter(log.message))
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::ImageUVFramePadding(m_logsIcon[static_cast<size_t>(log.type)].first->texture);
			ImGui::TableSetColumnIndex(1);
			ImGui::AlignTextToFramePadding();
			ImGui::Text(log.time);
			ImGui::TableSetColumnIndex(2);
			ImGui::AlignTextToFramePadding();
			ImGui::TextWrapped(log.message);
			ImGui::TableSetColumnIndex(3);
			if (ImGui::SmallButton(ICON_FA_COPY))
				ImGui::SetClipboardText(log.message);
			ImGui::HelpMarkerItem("Copy to Clipboard");
		}
	}

	void WindowConsole::ConsolePrint()
	{
		const Core::Debug::LogData* logs{ Core::Debug::Logger::GetLogsData() };
		const size_t& logSize{ Core::Debug::Logger::GetLogsDataSize() };

		if (ImGui::BeginTable("## ConsolePrint", 4, m_tableFlags, ImGui::GetContentRegionAvail()))
		{
			ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, ImGui::GetFontSize() * 3);
			ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed, ImGui::GetFontSize() * 4);
			ImGui::TableSetupColumn("Description");
			ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_WidthFixed, ImGui::GetFontSize() * 2.f);
			ImGui::TableAutoHeaders();

			for (int i{ 0 }; i < logSize; ++i)
			{
				ImGui::PushID(i);
				PrintLog(logs[i]);
				ImGui::PopID();
			}

			if (m_canScrollBottom)
			{
				ImGui::SetScrollHereY();
				m_canScrollBottom = false;
			}

			ImGui::EndTable();
		}
	}

	void WindowConsole::ClearOnPlay()
	{
		if (m_clearOnPlay && !m_consoleCleared && GetEngine()->IsPlaying())
		{
			Core::Debug::Logger::ClearLogs();
			m_consoleCleared = true;
		}
		if (m_consoleCleared && !GetEngine()->IsPlaying())
			m_consoleCleared = false;
	}

	void WindowConsole::Tick()
	{
		ClearOnPlay();

		ConsoleHeader();
		ImGui::Separator();
		ConsolePrint();
	}
}
