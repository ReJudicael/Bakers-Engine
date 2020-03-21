#include "WindowConsole.h"

namespace Editor::Window
{
	WindowConsole::WindowConsole(Canvas* canvas, bool visible) :
		AWindow{ canvas, "Console", visible }
	{
	}

	void WindowConsole::PushWindowStyle()
	{
	}

	void WindowConsole::PopWindowStyle()
	{
	}

	void WindowConsole::Tick()
	{
		ImGui::Text(GetNameID().c_str());
	}
}
