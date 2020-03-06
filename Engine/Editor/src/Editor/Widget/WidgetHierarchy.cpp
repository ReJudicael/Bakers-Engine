#include "WidgetHierarchy.h"

namespace Editor::Widget
{
	WidgetHierarchy::WidgetHierarchy() :
		IWidget(W_HIERARCHY)
	{
	}

	void WidgetHierarchy::Tick()
	{
		ImGui::Text(W_HIERARCHY);
	}
}
