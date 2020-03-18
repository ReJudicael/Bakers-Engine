#include "WindowInspector.h"

namespace Editor::Window
{
	WindowInspector::WindowInspector(EditorEngine* engine, bool visible) :
		AWindow{ engine, "Inspector", visible }
	{
	}

	void WindowInspector::Tick()
	{
		ImGui::Text(GetNameID().c_str());
	}
}
