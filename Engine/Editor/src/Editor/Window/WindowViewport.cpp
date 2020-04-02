#include "WindowViewport.h"
#include "EditorEngine.h"
#include "ICamera.h"

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
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });
	}

	void WindowViewport::PopWindowStyle()
	{
		ImGui::PopStyleVar(1);
	}

	void WindowViewport::DisplayViewport()
	{
		Core::Renderer::Framebuffer* fbo{ GetEngine()->GetFBO(0) };
		ImVec2 windowSize{ ImGui::GetContentRegionAvail() };

		if (fbo->Size[2] != windowSize.x || fbo->Size[3] != windowSize.y)
		{
			Core::Datastructure::ICamera* cam = reinterpret_cast<Core::Datastructure::ICamera*>(fbo->userPtr);
			if (cam != nullptr)
				cam->Resize(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));
			else
				fbo->Resize(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));
		}

#pragma warning(suppress : 4312)
		ImGui::ImageUV(reinterpret_cast<ImTextureID>(fbo->ColorTexture), windowSize);
	}

	void WindowViewport::Tick()
	{
		DisplayViewport();
	}
}
