#include "WidgetHierarchy.h"

namespace Editor::Widget
{
	WidgetHierarchy::WidgetHierarchy() :
		IWidget("Hierarchy")
	{
	}

	void WidgetHierarchy::Tick()
	{
		ImGui::Text(m_name.c_str());
	}
}
