#include "WidgetConsole.h"

namespace Editor::Widget
{
	WidgetConsole::WidgetConsole(const char* name) :
		IWidget(name)
	{
	}

	void WidgetConsole::Tick()
	{
		ImGui::Text(m_name);
	}
}
