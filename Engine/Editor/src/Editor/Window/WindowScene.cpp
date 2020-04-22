#include "WindowScene.h"
#include "ICamera.h"
#include "Framebuffer.h"
#include "EditorEngine.h"
#include "RootObject.hpp"

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
		if (m_cam)
		{
			m_cam->Update(ImGui::GetIO().DeltaTime);
			Core::Renderer::Framebuffer* fbo{ m_cam->GetFBO() };
			ImVec2 windowSize{ ImGui::GetContentRegionAvail() };

			if (fbo->Size[2] != windowSize.x || fbo->Size[3] != windowSize.y)
			{
				//To redo with the scene camera;
				m_cam->Resize(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));
			}

			ImGui::ImageUV(fbo->ColorTexture, windowSize);
		}
		else
		{
			ImGui::AlignTextToFramePadding();
			ImGui::Text("  No scene camera available");
		}
	}

	void WindowScene::Tick()
	{
		DisplayScene();
	}
}
