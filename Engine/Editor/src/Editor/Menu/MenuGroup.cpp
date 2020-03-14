#include "MenuGroup.h"
#include <imgui\imgui.h>

namespace Editor::Menu
{
	MenuGroup::MenuGroup(const std::string& name, bool enabled) :
		AMenu{ name },
		isEnabled{ enabled }
	{
	}

	MenuGroup::~MenuGroup()
	{
		if (m_menuGroupBegun)
			End();
	}

	bool MenuGroup::Begin()
	{
		m_menuGroupBegun = ImGui::BeginMenu(GetNameID().c_str(), isEnabled);
		return m_menuGroupBegun;
	}

	void MenuGroup::End()
	{
		ImGui::EndMenu();
		m_menuGroupBegun = false;
	}

	void MenuGroup::RemoveMenu(const AMenu& menu)
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

	void Editor::Menu::MenuGroup::RemoveAllMenus()
	{
		m_menus.clear();
	}

	void MenuGroup::Draw()
	{
		if (Begin())
		{
			for (auto& menu : m_menus)
				menu->Draw();

			End();
		}
	}
}
