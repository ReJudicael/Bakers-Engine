#include "WidgetInspector.h"

namespace Editor::Widget
{
	WidgetInspector::WidgetInspector(const char* name) :
		IWidget(name)
	{
	}

	void WidgetInspector::Tick()
	{
		ImGui::Text(m_name);
	}
}
