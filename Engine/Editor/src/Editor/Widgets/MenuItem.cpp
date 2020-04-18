#include <imgui\imgui.h>
#include "MenuItem.h"

namespace Editor::Widget
{
	MenuItem::MenuItem(const std::string& name, const std::string& shortcut, bool* selected, bool enabled) :
		AWidget{ name },
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
