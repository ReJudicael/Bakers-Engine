#include "WindowScene.h"
#include "ICamera.h"
#include "Framebuffer.h"
#include "EditorEngine.h"
#include "RootObject.hpp"
#include "Object.hpp"
#include "ImGuizmo.h"

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
			Core::Renderer::Framebuffer* fbo{ m_cam->GetFBO() };
			ImVec2 windowSize{ ImGui::GetContentRegionAvail() };

			if (fbo->Size[2] != windowSize.x || fbo->Size[3] != windowSize.y)
			{
				//To redo with the scene camera;
				m_cam->Resize(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));
			}

			ImGui::ImageUV(fbo->ColorTexture, windowSize);

			if (GetEngine()->objectSelected && GetEngine()->operation != SelectionMode::MOVEMENT)
			{
				ImGuiIO& io = ImGui::GetIO();
				ImGuizmo::SetDrawlist();
				ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
				ImGuizmo::ViewManipulate((float*)m_cam->GetCameraMatrix().array, 0.f, ImVec2(0, 0), ImVec2(0, 0), 0x10101010);
				GizmoManipulateResult();
			}
		}
		else
		{
			ImGui::AlignTextToFramePadding();
			ImGui::Text("  No scene camera available");
		}
	}

	void WindowScene::GizmoManipulateResult()
	{
		Core::Maths::Mat4 transform = GetEngine()->objectSelected->GetGlobalTRS();
		float* transformMatrix = transform.mat.Transposed().array;
		float t[3], r[3], s[3], t2[3], r2[3], s2[3];
		ImGuizmo::DecomposeMatrixToComponents(transformMatrix, t, r, s);
		Core::Maths::Mat<4, 4> view = m_cam->GetCameraMatrix().mat.Transposed();
		ImGuizmo::Manipulate(view.array, m_cam->GetPerspectiveMatrix().array, (ImGuizmo::OPERATION)GetEngine()->operation, GetEngine()->gizmoMode, transformMatrix);
		ImGuizmo::DecomposeMatrixToComponents(transformMatrix, t2, r2, s2);
		
		switch (GetEngine()->operation)
		{
		case SelectionMode::TRANSLATION:
		{
			Core::Maths::Vec3 translate(t2[0] - t[0], t2[1] - t[1], t2[2] - t[2]);
			if (translate.Length() > 0)
			{
				translate += GetEngine()->objectSelected->GetGlobalPos();
				GetEngine()->objectSelected->SetGlobalPos(translate);
			}
			break;
		}
		case SelectionMode::ROTATION:
		{
			Core::Maths::Vec3 eulerRotate(r2[0] - r[0], r2[1] - r[1], r2[2] - r[2]);
			if (eulerRotate.Length() > 0)
			{
				Core::Maths::Quat rotate(eulerRotate);
				rotate *= GetEngine()->objectSelected->GetGlobalRot();
				GetEngine()->objectSelected->SetGlobalRot(rotate);
			}
			break;
		}
		case SelectionMode::SCALE:
		{
			Core::Maths::Vec3 scale(s2[0] - s[0], s2[1] - s[1], s2[2] - s[2]);
			if (scale.Length() > 0)
			{
				scale += GetEngine()->objectSelected->GetGlobalScale();
				GetEngine()->objectSelected->SetGlobalScale(scale);
			}
			break;
		}
		default:break;
		}
	}

	void WindowScene::Tick()
	{
		DisplayScene();
	}
}
