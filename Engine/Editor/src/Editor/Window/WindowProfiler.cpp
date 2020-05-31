#include "WindowProfiler.h"
#include <glad\glad.h>

#include "IconsFontAwesome5.h"

namespace Editor::Window
{
	WindowProfiler::WindowProfiler(Canvas* canvas, bool visible) :
		AWindow{ canvas, ICON_FA_CHART_BAR "  Profiler", visible }
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
		if (ImGui::CollapsingHeader("GPU Infos", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			ImGui::Text("GL_VERSION: %s", glGetString(GL_VERSION));
			ImGui::Text("GL_RENDERER: %s", glGetString(GL_RENDERER));
			ImGui::Text("GL_SHADING_LANGUAGE_VERSION: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
		}
	}
}