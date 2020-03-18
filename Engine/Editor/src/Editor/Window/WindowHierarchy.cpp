#include "WindowHierarchy.h"

namespace Editor::Window
{
	WindowHierarchy::WindowHierarchy(EditorEngine* engine, bool visible) :
		AWindow{ engine, "Hierarchy", visible }
	{
	}

	void WindowHierarchy::Tick()
	{
		ImGui::Text(GetNameID().c_str());
	}
}
