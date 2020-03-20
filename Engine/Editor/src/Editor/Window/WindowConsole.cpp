#include "WindowConsole.h"

namespace Editor::Window
{
	WindowConsole::WindowConsole(Canvas* parent, bool visible) :
		AWindow{ parent, "Console", visible }
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
