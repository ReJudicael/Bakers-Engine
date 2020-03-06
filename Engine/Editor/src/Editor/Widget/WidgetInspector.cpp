#include "WidgetInspector.h"

namespace Editor::Widget
{
	WidgetInspector::WidgetInspector() :
		IWidget(W_INSPECTOR)
	{
	}

	void WidgetInspector::Tick()
	{
		ImGui::Text(W_INSPECTOR);
	}
}
