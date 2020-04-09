#include "WindowScene.h"
#include "EditorEngine.h"
#include "ICamera.h"
#include "RootObject.hpp"

#include <string>

namespace Editor::Window
{
	WindowScene::WindowScene(Canvas* canvas, bool visible) :
		AWindow{ canvas, "Scene", visible }
	{
		m_flags |= ImGuiWindowFlags_NoScrollbar;

		//Should be replaced by scene camera code
		//m_fbo = GetEngine()->CreateFBO();
		m_cam = new Datastructure::EditorCamera();
		GetEngine()->m_root->AddComponent(m_cam);
	}

	void WindowScene::PushWindowStyle()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });
	}

	void WindowScene::PopWindowStyle()
	{
		ImGui::PopStyleVar(1);
	}

	void WindowScene::DisplayScene()
	{
		if (m_cam == nullptr)
		{
			ImGui::AlignTextToFramePadding();
			ImGui::Text("  No scene camera available");
		}
		else
		{
			Core::Renderer::Framebuffer* fbo{ m_cam->GetFBO() };
			ImVec2 windowSize{ ImGui::GetContentRegionAvail() };

			if (fbo->Size[2] != windowSize.x || fbo->Size[3] != windowSize.y)
			{
				//To redo with the scene camera;
				m_cam->Resize(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));
			}

#pragma warning(suppress : 4312)
			ImGui::ImageUV(reinterpret_cast<ImTextureID>(fbo->ColorTexture), windowSize);
		}
	}

	void WindowScene::Tick()
	{
		DisplayScene();
	}
}
