#include "WindowScene.h"
#include "ICamera.h"
#include "Framebuffer.h"
#include "EditorEngine.h"
#include "RootObject.hpp"
#include "ImGuizmo.h"

namespace Editor::Window
{
	Core::Maths::Mat<4, 4> matrix{ matrix.Identity() };

	WindowScene::WindowScene(Canvas* canvas, bool visible) :
		AWindow{ canvas, "Scene", visible }
	{
		m_flags |= ImGuiWindowFlags_NoScrollbar;

		//Should be replaced by scene camera code
		//m_fbo = GetEngine()->CreateFBO();
		m_cam = new Datastructure::EditorCamera();
		GetEngine()->m_root->AddComponent(m_cam);
		
		matrix.m_mat[14] = -2;
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
			ImGuiIO& io = ImGui::GetIO();
			ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
			
			if (GetEngine()->objectSelected)
			{
				Core::Maths::Mat4 transform = GetEngine()->objectSelected->GetGlobalTRS();
				float t[3], r[3], s[3], t2[3], r2[3], s2[3];
				ImGuizmo::DecomposeMatrixToComponents(matrix.m_mat, t, r, s);
				ImGuizmo::DrawGrid(m_cam->GetCameraMatrix().m_array, m_cam->GetPerspectiveMatrix().m_array, matrix.m_mat, 1);
				ImGuizmo::Manipulate(m_cam->GetCameraMatrix().m_array, m_cam->GetPerspectiveMatrix().m_array, ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::MODE::LOCAL, matrix.m_mat);
				ImGuizmo::DecomposeMatrixToComponents(matrix.m_mat, t2, r2, s2);
				Core::Maths::Vec3 translate(t2[0] - t[0], t2[1] - t[1], t2[2] - t[2]);
				if (translate.Length() > 0)
				{
					translate += GetEngine()->objectSelected->GetGlobalPos();
					GetEngine()->objectSelected->SetGlobalPos(translate);
				}
			}

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
