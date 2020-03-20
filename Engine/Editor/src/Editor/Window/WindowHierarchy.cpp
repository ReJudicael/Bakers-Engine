#include "WindowHierarchy.h"

namespace Editor::Window
{
	WindowHierarchy::WindowHierarchy(Canvas* parent, bool visible) :
		AWindow{ parent, "Hierarchy", visible }
	{
	}

	void WindowHierarchy::PushWindowStyle()
	{
	}

	void WindowHierarchy::PopWindowStyle()
	{
	}

	void WindowHierarchy::Tick()
	{
		ImGui::Text(GetNameID().c_str());
	}
}
