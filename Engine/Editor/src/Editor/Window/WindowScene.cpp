#include "WindowScene.h"
#include "EditorEngine.h"

#include <string>

namespace Editor::Window
{
	WindowViewport::WindowViewport(EditorEngine* engine, bool visible) :
		AWindow{ engine, "Viewport", visible }
	{
		m_flags |= ImGuiWindowFlags_NoScrollbar;
	}

	void WindowViewport::Tick()
	{
		Framebuffer& fbo{ m_engine->GetFBO() };
		ImVec2 windowSize{ ImGui::GetContentRegionAvail() };
		if (fbo.Size[2] != windowSize.x || fbo.Size[3] != windowSize.y)
		{
			m_engine->ResizeFBO(windowSize.x, windowSize.y);
		}
		ImGui::Image((ImTextureID)fbo.ColorTexture, windowSize);
	}
}
