#include "WindowConsole.h"

namespace Editor::Window
{
	WindowConsole::WindowConsole() :
		AWindow("Console")
	{
	}

	void WindowConsole::Tick()
	{
		ImGui::Text(GetNameID().c_str());
	}
}
