#include "WidgetConsole.h"

namespace Editor::Widget
{
	WidgetConsole::WidgetConsole() :
		IWidget("Console")
	{
	}

	void WidgetConsole::Tick()
	{
		ImGui::Text(m_nameID.c_str());
	}
}
