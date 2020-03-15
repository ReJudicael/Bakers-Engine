#include "WindowHierarchy.h"

namespace Editor::Window
{
	WindowHierarchy::WindowHierarchy() :
		AWindow("Hierarchy")
	{
	}

	void WindowHierarchy::Tick()
	{
		ImGui::Text(GetNameID().c_str());
	}
}
