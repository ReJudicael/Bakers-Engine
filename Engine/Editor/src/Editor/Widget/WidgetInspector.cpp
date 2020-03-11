#include "WidgetInspector.h"

namespace Editor::Widget
{
	WidgetInspector::WidgetInspector() :
		AWidget("Inspector")
	{
	}

	void WidgetInspector::Tick()
	{
		ImGui::Text(GetNameID().c_str());
	}
}
