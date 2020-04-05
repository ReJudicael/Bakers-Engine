#include "WindowConsole.h"
#include "EditorEngine.h"

namespace Editor::Window
{
	WindowConsole::WindowConsole(Canvas* canvas, bool visible) :
		AWindow{ canvas, "Console", visible }
	{
		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ConsoleIcons\\error.png", m_icons[0]);
		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ConsoleIcons\\warning.png", m_icons[1]);
		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ConsoleIcons\\message.png", m_icons[2]);
	}

	void WindowConsole::PushWindowStyle()
	{
	}

	void WindowConsole::PopWindowStyle()
	{
	}

	void WindowConsole::AddLogButton(const std::shared_ptr<Resources::Texture>& icon, const std::string& label, bool& isEnabled)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, isEnabled ? ImVec4(0.18f, 0.32f, 0.45f, 1.0f) : ImGui::GetStyle().Colors[ImGuiCol_FrameBg]);
#pragma warning(suppress : 4312)
		if (ImGui::ImageButtonUVWithText_HelpMarker(reinterpret_cast<ImTextureID>(icon->texture), label, label.c_str()))
			isEnabled = !isEnabled;
		ImGui::PopStyleColor();
	}

	void WindowConsole::ConsoleHeader()
	{
		ImGui::ColoredButton("Clear", { 50.f, 0.f }, { 0.75f, 0.22f, 0.17f });
		ImGui::SameLine();

		std::string errors{ std::to_string(5) + " Errors" };
		AddLogButton(m_icons[0], m_iconIsEnabled[0] ? errors : "0 of " + errors, m_iconIsEnabled[0]);

		ImGui::SameLine();
		std::string warnings{ std::to_string(120) + " Warnings" };
		AddLogButton(m_icons[1], m_iconIsEnabled[1] ? warnings : "0 of " + warnings, m_iconIsEnabled[1]);

		ImGui::SameLine();
		std::string messages{ std::to_string(47) + " Messages" };
		AddLogButton(m_icons[2], m_iconIsEnabled[2] ? messages : "0 of " + messages, m_iconIsEnabled[2]);

		ImGui::SameLine();
		m_textFilter.Draw("Filter", ImGui::GetContentRegionAvail().x - 37.f);
	}

	void WindowConsole::Tick()
	{
		ConsoleHeader();
		ImGui::Separator();
	}
}
