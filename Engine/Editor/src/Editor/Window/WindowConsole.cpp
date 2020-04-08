#include "WindowConsole.h"
#include "EditorEngine.h"

namespace Editor::Window
{
	WindowConsole::WindowConsole(Canvas* canvas, bool visible) :
		AWindow{ canvas, "Console", visible }
	{
		for (int i = 0; i < static_cast<int>(Core::Debug::LogType::COUNT); ++i)
			m_log[i].isEnabled = true;

		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ConsoleIcons\\message.png", m_log[0].icon);
		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ConsoleIcons\\warning.png", m_log[1].icon);
		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ConsoleIcons\\error.png", m_log[2].icon);
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
		if (ImGui::ImageButtonUVWithText_HelpMarker(reinterpret_cast<ImTextureID>(icon->texture), "## LogButton", label, help_marker.c_str()))
			isEnabled = !isEnabled;
		ImGui::PopStyleColor();
	}

	void WindowConsole::ConsoleHeader()
	{
		if (ImGui::ColoredButton("Clear", { 50.f, 0.f }, { 0.75f, 0.22f, 0.17f }))
			Core::Debug::Logger::ClearLogs();

		std::string label, nbLogs, typeLog;
		for (int i{ static_cast<int>(Core::Debug::LogType::COUNT) - 1 }; i >= 0; --i)
		{
			nbLogs	= std::to_string(m_log[i].nbLogs);
			label	= m_log[i].isEnabled ? nbLogs : "0 of " + nbLogs;
			typeLog	= ToString(static_cast<Core::Debug::LogType>(i));

			ImGui::PushID(i);
			ImGui::SameLine();
			AddLogButton(m_log[i].icon, label, typeLog, m_log[i].isEnabled);
			ImGui::PopID();
		}

		ImGui::SameLine();
		m_textFilter.Draw("Filter", ImGui::GetContentRegionAvail().x - 37.f);
	}

	void WindowConsole::LogPrint(Core::Debug::LogData log)
	{
		++m_log[(size_t)log.type].nbLogs;
		if (m_log[(size_t)log.type].isEnabled && m_textFilter.PassFilter(log.message))
		{
			ImGui::ImageUV(reinterpret_cast<ImTextureID>(m_log[(size_t)log.type].icon->texture), { 16.f });
			ImGui::SameLine();
			ImGui::Text("%s\t-\t", log.time);
			ImGui::SameLine();
			ImGui::Text("%s", log.message);
			ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 40.f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0, 0 });
			ImGui::ButtonEx("1", { 32, 16 }, ImGuiButtonFlags_Disabled);
			ImGui::PopStyleVar(1);
			ImGui::Separator();
		}
	}

	void WindowConsole::ConsolePrint()
	{
		Core::Debug::LogData* logs = Core::Debug::Logger::GetLogdata();
		int logSize = Core::Debug::Logger::GetLogdataSize();

		for (int i{ 0 }; i < static_cast<int>(Core::Debug::LogType::COUNT); ++i)
			m_log[i].nbLogs = 0;

		if (ImGui::BeginChild("## ConsolePrint", { 0.f }, false, ImGuiWindowFlags_HorizontalScrollbar))
		{
			ImGui::Spacing();
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 8, 8 });
			for (int i = 0; i < logSize; ++i)
				LogPrint(logs[i]);
			ImGui::PopStyleVar();
		}
	}

	void WindowConsole::Tick()
	{
		ConsoleHeader();
		ImGui::Separator();
		ConsolePrint();

		// TODO: Delete this
		if (GetEngine()->GetInputSystem()->IsMouseButtonPressed(EMouseButton::B2))
			DEBUG_LOG_ERROR(
				"Lorem ipsum dolor sit amet, consetetur sadipscing elitr,\nsed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat,\nsed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum.");
	}
}
