#include "MenuItem.h"
#include <imgui\imgui.h>

namespace Editor::Menu
{
	MenuItem::MenuItem(const std::string& name, const std::string& shortcut, bool* selected, bool enabled) :
		AMenu{ name },
		m_shortcut{ shortcut },
		m_selected{ selected },
		isEnabled{ enabled }
	{
	}
	
	bool MenuItem::Begin()
	{
		return ImGui::MenuItem(GetNameID().c_str(), m_shortcut.c_str(), m_selected, isEnabled);
	}

	void MenuItem::Draw()
	{
		if (Begin())
			OnClick();
	}
}
