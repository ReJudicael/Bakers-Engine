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

	void MenuBar::RemoveMenu(const AMenu& menu)
	{
		auto found = std::find_if(m_menus.begin(), m_menus.end(),
			[&menu](std::unique_ptr<AMenu>& m)
			{
				return m.get() == &menu;
			});

		if (found != m_menus.end())
		{
			m_menus.erase(found);
		}
	}

	void MenuBar::RemoveAllMenus()
	{
		m_menus.clear();
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
			for (auto& menu : m_menus)
				menu->Draw();
			End();
		}
	}
}
