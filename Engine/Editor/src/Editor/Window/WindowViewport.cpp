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
		int camNum = GetEngine()->GetFBONum(Core::Renderer::FBOType::CAMERA);
		if (m_cameraNum >= camNum)
			m_cameraNum = camNum - 1;
		if (camNum > 0 && ImGui::BeginCombo("test", std::to_string(m_cameraNum).c_str()))
		{
			for (int i = 0; i < camNum; ++i)
			{
				if (ImGui::MenuItem(("Viewport " + std::to_string(i)).c_str()))
					m_cameraNum = i;
			}
			ImGui::EndCombo();
		}
		Core::Renderer::Framebuffer* fbo{ GetEngine()->GetFBO(m_cameraNum, Core::Renderer::FBOType::CAMERA) };
		if (fbo)
		{
			ImVec2 windowSize{ ImGui::GetContentRegionAvail() };

			if (fbo->Size[2] != windowSize.x || fbo->Size[3] != windowSize.y)
			{
				Core::Datastructure::ICamera* cam = reinterpret_cast<Core::Datastructure::ICamera*>(fbo->userPtr);
				if (cam != nullptr)
					cam->Resize(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));
				else
					fbo->Resize(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));
			}

			ImGui::ImageUV(fbo->ColorTexture, windowSize);
		}
		else
		{
			ImGui::AlignTextToFramePadding();
			ImGui::Text("  No viewport available");
		}
	}

	void WindowViewport::Tick()
	{
		DisplayViewport();
	}
}
