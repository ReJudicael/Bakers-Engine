#include "WindowConsole.h"

namespace Editor::Window
{
	WindowConsole::WindowConsole(bool visible) :
		AWindow{ "Console" }
	{
		isVisible = visible;
	}

	void WindowConsole::Tick()
	{
		ImGui::Text(GetNameID().c_str());
	}
}
