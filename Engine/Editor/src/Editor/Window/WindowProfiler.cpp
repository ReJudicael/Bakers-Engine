#include "WindowProfiler.h"

namespace Editor::Window
{
	WindowProfiler::WindowProfiler(Canvas* canvas, bool visible) :
		AWindow{ canvas, "Profiler", visible }
	{
	}

	void WindowProfiler::PushWindowStyle()
	{
	}

	void WindowProfiler::PopWindowStyle()
	{
	}

	void WindowProfiler::Tick()
	{
		ImGui::Text(GetNameID().c_str());
	}
}