#include "WidgetHierarchy.h"

namespace Editor::Widget
{
	WidgetHierarchy::WidgetHierarchy() :
		AWidget("Hierarchy")
	{
	}

	void WidgetHierarchy::Tick()
	{
		ImGui::Text(GetNameID().c_str());
	}
}
