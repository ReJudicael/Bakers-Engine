#include "WindowInspector.h"

namespace Editor::Window
{
	WindowInspector::WindowInspector(bool visible) :
		AWindow{ "Inspector" }
	{
		isVisible = visible;
	}

	void WindowInspector::Tick()
	{
		ImGui::Text(GetNameID().c_str());
	}
}
