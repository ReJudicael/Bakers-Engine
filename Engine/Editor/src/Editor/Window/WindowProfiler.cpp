#include "WindowProfiler.h"

namespace Editor::Window
{
	WindowProfiler::WindowProfiler(EditorEngine* engine, bool visible) :
		AWindow{ engine, "Profiler", visible }
	{
	}

	void WindowProfiler::Tick()
	{
		ImGui::Text(GetNameID().c_str());
	}
}