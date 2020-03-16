#include "WidgetInspector.h"

namespace Editor::Widget
{
	WidgetInspector::WidgetInspector() :
		IWidget("Inspector")
	{
	}

	void WidgetInspector::Tick()
	{
		ImGui::Text(m_nameID.c_str());
	}
}
