#include "WidgetConsole.h"

namespace Editor::Widget
{
	WidgetConsole::WidgetConsole() :
		AWidget("Console")
	{
	}

	void WidgetConsole::Tick()
	{
		ImGui::Text(GetNameID().c_str());
	}
}
