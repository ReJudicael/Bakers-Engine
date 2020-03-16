#include "WindowHierarchy.h"

namespace Editor::Window
{
	WindowHierarchy::WindowHierarchy(bool visible) :
		AWindow{ "Hierarchy" }
	{
		isVisible = visible;
	}

	void WindowHierarchy::Tick()
	{
		ImGui::Text(GetNameID().c_str());
	}
}
