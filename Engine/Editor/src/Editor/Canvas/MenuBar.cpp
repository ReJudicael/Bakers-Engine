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
			for (auto& menu : m_contents)
				menu->Draw();
			End();
		}
	}
}
