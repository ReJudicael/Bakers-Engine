#include "WindowProfiler.h"

namespace Editor::Window
{
	WindowProfiler::WindowProfiler() :
		AWindow("Profiler")
	{
	}

	void WindowProfiler::Tick()
	{
		ImGui::Text(GetNameID().c_str());
	}
}