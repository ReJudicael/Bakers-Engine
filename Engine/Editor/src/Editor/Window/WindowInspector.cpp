#include "WindowInspector.h"

namespace Editor::Window
{
	WindowInspector::WindowInspector() :
		AWindow("Inspector")
	{
	}

	void WindowInspector::Tick()
	{
		ImGui::Text(GetNameID().c_str());
	}
}
