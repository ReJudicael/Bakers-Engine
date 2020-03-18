#include "WindowConsole.h"

namespace Editor::Window
{
	WindowConsole::WindowConsole(EditorEngine* engine, bool visible) :
		AWindow{ engine, "Console", visible }
	{
	}

	void WindowConsole::Tick()
	{
		ImGui::Text(GetNameID().c_str());
	}
}
