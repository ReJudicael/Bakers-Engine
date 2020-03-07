#include "WidgetInspector.h"

namespace Editor::Widget
{
	WidgetInspector::WidgetInspector() :
		IWidget("Inspector")
	{
	}

	void WidgetInspector::Tick()
	{
		ImGui::Text(m_name.c_str());
	}
}
