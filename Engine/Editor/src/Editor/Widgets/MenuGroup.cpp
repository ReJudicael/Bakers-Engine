#include <imgui\imgui.h>
#include "MenuGroup.h"

namespace Editor::Widget
{
	MenuGroup::MenuGroup(const std::string& name, bool enabled) :
		AWidget{ name },
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

	void MenuGroup::Draw()
	{
		if (Begin())
		{
			for (auto& menu : m_contents)
				menu->Draw();

			End();
		}
	}
}
