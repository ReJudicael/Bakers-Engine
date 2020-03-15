#include "MenuBar.h"
#include <imgui\imgui.h>

#include <iostream>

namespace Editor::Menu
{
	MenuBar::~MenuBar()
	{
		if (m_menuBarBegun)
			End();
	}

	bool MenuBar::Begin()
	{
		m_menuBarBegun = ImGui::BeginMainMenuBar();
		return m_menuBarBegun;
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
