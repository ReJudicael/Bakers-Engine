#include "WindowProfiler.h"

namespace Editor::Window
{
	WindowProfiler::WindowProfiler(bool visible) :
		AWindow{ "Profiler" }
	{
		isVisible = visible;
	}

	void WindowProfiler::Tick()
	{
		ImGui::Text(GetNameID().c_str());
	}
}