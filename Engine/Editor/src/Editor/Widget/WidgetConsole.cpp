#include "WidgetConsole.h"

namespace Editor::Widget
{
	WidgetConsole::WidgetConsole() :
		IWidget(W_CONSOLE)
	{
	}

	void WidgetConsole::Tick()
	{
		ImGui::Text(W_CONSOLE);
	}
}
