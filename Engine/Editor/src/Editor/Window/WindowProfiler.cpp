#include "WindowProfiler.h"

namespace Editor::Window
{
	WindowProfiler::WindowProfiler(Canvas* parent, bool visible) :
		AWindow{ parent, "Profiler", visible }
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