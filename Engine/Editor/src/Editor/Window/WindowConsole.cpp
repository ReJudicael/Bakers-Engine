#include "WindowConsole.h"
#include "EditorEngine.h"

namespace Editor::Window
{
	WindowConsole::WindowConsole(Canvas* canvas, bool visible) :
		AWindow{ canvas, "Console", visible }
	{
		for (ConsoleLogDataType& log : m_log)
			log.isEnabled = true;

		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ConsoleIcons\\message.png", m_log[0].icon);
		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ConsoleIcons\\warning.png", m_log[1].icon);
		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ConsoleIcons\\error.png", m_log[2].icon);
		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ConsoleIcons\\settings.png", m_settingsIcon);

		Core::Debug::Logger::OnLogAdded += BIND_EVENT(WindowConsole::LogCallback);
	}

	void WindowConsole::PushWindowStyle()
	{
	}

	void WindowConsole::PopWindowStyle()
	{
	}

	void WindowConsole::AddLogButton(const std::shared_ptr<Resources::Texture>& icon, const std::string& label, const std::string& help_marker, bool& isEnabled)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, isEnabled ? ImVec4(0.18f, 0.32f, 0.45f, 1.0f) : ImVec4(0.18f, 0.18f, 0.25f, 1.0f));
#pragma warning(suppress : 4312)
		if (ImGui::ImageButtonUVWithText_HelpMarker(icon->texture, "## LogButton", label, help_marker.c_str()))
			isEnabled = !isEnabled;
		ImGui::PopStyleColor();
	}

	void WindowConsole::SettingsButton()
	{
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.18f, 0.18f, 0.25f, 1.0f });
		ImGui::ImageButtonUV_HelpMarker(m_settingsIcon->texture, "Settings");
		ImGui::PopStyleColor();

		if (ImGui::IsItemClicked())
			ImGui::OpenPopup("## Settings");

		if (ImGui::BeginPopup("## Settings"))
		{
			ImGui::MenuItem("Auto-scroll", (const char*)0, &m_autoScroll);
			ImGui::MenuItem("Clear on Play", (const char*)0, nullptr);
			ImGui::EndPopup();
		}
	}

	void WindowConsole::ConsoleHeader()
	{
		if (ImGui::ColoredButton("Clear", { 50.f, 0.f }, { 0.75f, 0.22f, 0.17f }))
		{
			Core::Debug::Logger::ClearLogs();
			for (auto& log : m_log) { log.nbLogs = 0; };
		}

		std::string label, nbLogs, typeLog;
		for (int i{ 2 }; i >= 0; --i)
		{
			nbLogs	= std::to_string(m_log[i].nbLogs);
			label	= m_log[i].isEnabled ? nbLogs : "0 of " + nbLogs;
			typeLog	= ToString(static_cast<Core::Debug::ELogType>(i));

			ImGui::SameLine();
			ImGui::PushID(i);
			AddLogButton(m_log[i].icon, label, typeLog, m_log[i].isEnabled);
			ImGui::PopID();
		}

		ImGui::SameLine();
		SettingsButton();

		ImGui::SameLine();
		m_textFilter.Draw("Filter", ImGui::GetContentRegionAvail().x - 37.f);
	}

	void WindowConsole::PrintLog(Core::Debug::LogData log)
	{
		if (m_log[(size_t)log.type].isEnabled && m_textFilter.PassFilter(log.message))
		{
			ImGui::ImageUV(m_log[(size_t)log.type].icon->texture);
			ImGui::SameLine();
			ImGui::Text("%s\t-\t%s", log.time, log.message);
			ImGui::Separator();
		}
	}

	void WindowConsole::LogCallback(const Core::Debug::LogData& log_data)
	{
		++m_log[static_cast<size_t>(log_data.type)].nbLogs;

		if (m_autoScroll)
			m_canScrollBottom = true;
	}

	void WindowConsole::ConsolePrint()
	{
		Core::Debug::LogData* logs = Core::Debug::Logger::GetLogsData();
		int logSize = Core::Debug::Logger::GetLogsDataSize();

		if (ImGui::BeginChild("## ConsolePrint", { 0.f }, false, ImGuiWindowFlags_HorizontalScrollbar))
		{
			ImGui::Spacing();
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 8.f, 8.f });
			for (int i{ 0 }; i < logSize; ++i)
				PrintLog(logs[i]);
			ImGui::PopStyleVar();

			if (m_canScrollBottom)
			{
				ImGui::SetScrollHereY();
				m_canScrollBottom = false;
			}

			ImGui::EndChild();
		}
	}

	void WindowConsole::Tick()
	{
		ConsoleHeader();
		ImGui::Separator();
		ConsolePrint();
	}
}
