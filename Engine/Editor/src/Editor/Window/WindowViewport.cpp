#include "WindowViewport.h"
#include "EditorEngine.h"

#include <string>

namespace Editor::Window
{
	WindowViewport::WindowViewport(Canvas* canvas, bool visible) :
		AWindow{ canvas, "Viewport", visible }
	{
		m_flags |= ImGuiWindowFlags_NoScrollbar;
	}

	void WindowViewport::PushWindowStyle()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
	}

	void WindowViewport::PopWindowStyle()
	{
		ImGui::PopStyleVar(1);
	}

	void WindowViewport::Tick()
	{
		Core::Renderer::Framebuffer* fbo{ GetEngine()->GetFBO(0) };
		ImVec2 windowSize{ ImGui::GetContentRegionAvail() };
		if (fbo->Size[2] != windowSize.x || fbo->Size[3] != windowSize.y)
		{
			fbo->Resize(windowSize.x, windowSize.y);
		}
		ImGui::Image((ImTextureID)fbo->ColorTexture, windowSize);
	}
}
