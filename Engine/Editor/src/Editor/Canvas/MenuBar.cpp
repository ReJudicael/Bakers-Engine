#include <imgui\imgui.h>
#include "MenuBar.h"

namespace Editor
{
	MenuBar::~MenuBar()
	{
		if (m_menuBarBegun)
			End();
	}

	bool MenuBar::Begin()
	{
		ImGui::BeginMainMenuBar();
		m_menuBarBegun = true;
		return true;
	}

	void MenuBar::End()
	{
		ImGui::EndMainMenuBar();
		m_menuBarBegun = false;
	}

	void MenuBar::Draw()
	{
		if (Begin())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 8.f, 4.f });
			for (auto& menu : m_contents)
				menu->Draw();
			ImGui::PopStyleVar();
			End();
		}
	}
}
