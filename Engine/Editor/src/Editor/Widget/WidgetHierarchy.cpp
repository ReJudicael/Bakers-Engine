#include "WidgetHierarchy.h"

namespace Editor::Widget
{
	WidgetHierarchy::WidgetHierarchy(const char* name) :
		IWidget(name)
	{
	}

	void WidgetHierarchy::Tick()
	{
		ImGui::Text(m_name);
	}
}
