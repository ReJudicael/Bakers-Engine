#include "WidgetHierarchy.h"

namespace Editor::Widget
{
	WidgetHierarchy::WidgetHierarchy() :
		IWidget("Hierarchy")
	{
	}

	void WidgetHierarchy::Tick()
	{
		ImGui::Text(m_nameID.c_str());
	}
}
