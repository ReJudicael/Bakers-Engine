#include "WidgetProfiler.h"

namespace Editor::Widget
{
	WidgetProfiler::WidgetProfiler() :
		AWidget("Profiler")
	{
	}

	void WidgetProfiler::Tick()
	{
		ImGui::Text(GetNameID().c_str());
	}
}