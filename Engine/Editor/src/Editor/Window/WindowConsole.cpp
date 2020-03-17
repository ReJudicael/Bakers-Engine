#include "WindowConsole.h"

namespace Editor::Window
{
	WindowConsole::WindowConsole(bool visible) :
		AWindow{ "Console", visible }
	{
	}

	void WindowConsole::Tick()
	{
		ImGui::Text(GetNameID().c_str());
	}
}
