#include "WindowScene.h"
#include "ICamera.h"
#include "Framebuffer.h"
#include "EditorEngine.h"
#include "RootObject.hpp"
#include "Object.hpp"
#include "ImGuizmo.h"
#include "Maths.hpp"

namespace Editor::Window
{
	WindowScene::WindowScene(Canvas* canvas, bool visible) :
		AWindow{ canvas, "Scene", visible }
	{
		m_flags |= ImGuiWindowFlags_NoScrollbar;

		m_cam = new Datastructure::EditorCamera();
		GetEngine()->m_root->AddComponent(m_cam);

		m_cube.array[14] = 10;
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
				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowSize.x, windowSize.y);
				
				CheckGizmoUse();
				GizmoViewManipulateResult();
				GizmoManipulateResult();
			}
		}
		else
		{
			ImGui::AlignTextToFramePadding();
			ImGui::Text("  No scene camera available");
		}
	}

	void WindowScene::CheckGizmoUse()
	{
		if (ImGuizmo::IsUsing())
			return;

		Core::Maths::Vec2 mouse = GetEngine()->GetMousePos();

		if (mouse.x < ImGui::GetWindowPos().x || mouse.x > ImGui::GetWindowPos().x + ImGui::GetWindowSize().x
			|| mouse.y < ImGui::GetWindowPos().y || mouse.y > ImGui::GetWindowPos().y + ImGui::GetWindowSize().y)
			ImGuizmo::Enable(false);
		else
			ImGuizmo::Enable(true);
	}

	void WindowScene::GizmoViewManipulateResult()
	{
		// Save transform values before gizmo manipulate
		Core::Maths::Vec3 t, r, s;
		ImGuizmo::DecomposeMatrixToComponents(m_cube.array, t.xyz, r.xyz, s.xyz);

		ImGuizmo::ViewManipulate(m_cube.array, 0.1f, ImGui::GetWindowPos(), ImVec2(128, 128), 0x10101010);
		
		// Get new values
		Core::Maths::Vec3 t2, r2, s2;
		ImGuizmo::DecomposeMatrixToComponents(m_cube.array, t2.xyz, r2.xyz, s2.xyz);

		// Check and apply position modification
		t = t2 - t;
		if (t.SquaredLength() > 0)
		{
			// Convert gizmo position vector into a vector usable in scene
			t2.x *= -1;
			t2.y *= -1;
			m_cam->SetPos(t2);
		}

		// Check and apply rotation modification
		r = r2 - r;
		if (r.SquaredLength() > 0)
		{
			// Convert gizmo rotation euler angles into a valid rotation vector
			r2.x = Core::Maths::ToRadians(-r2.x);
			r2.y = Core::Maths::ToRadians(-r2.y);
			r2.z = Core::Maths::ToRadians(r2.z);
			m_cam->SetRot(r2);
		}
	}

	void WindowScene::GizmoManipulateResult()
	{
		Core::Maths::Mat4 transform = GetEngine()->objectSelected->GetGlobalTRS();
		float* transformMatrix = transform.mat.Transposed().array;
		Core::Maths::Vec3 t, r, s, t2, r2, s2;
		ImGuizmo::DecomposeMatrixToComponents(transformMatrix, t.xyz, r.xyz, s.xyz);

		Core::Maths::Mat<4, 4> view = m_cam->GetCameraMatrix().mat.Transposed();
		ImGuizmo::Manipulate(view.array, m_cam->GetPerspectiveMatrix().array, (ImGuizmo::OPERATION)GetEngine()->operation, GetEngine()->gizmoMode, transformMatrix);
		ImGuizmo::DecomposeMatrixToComponents(transformMatrix, t2.xyz, r2.xyz, s2.xyz);
		
		switch (GetEngine()->operation)
		{
		case SelectionMode::TRANSLATION:
		{
			Core::Maths::Vec3 translate = t2 - t;
			if (translate.Length() > 0)
			{
				translate += GetEngine()->objectSelected->GetGlobalPos();
				GetEngine()->objectSelected->SetGlobalPos(translate);
			}
			break;
		}
		case SelectionMode::ROTATION:
		{
			Core::Maths::Vec3 eulerRotate = r2 - r;
			eulerRotate.x = Core::Maths::ToRadians(eulerRotate.x);
			eulerRotate.y = Core::Maths::ToRadians(eulerRotate.y);
			eulerRotate.z = Core::Maths::ToRadians(eulerRotate.z);
			Core::Maths::Vec3 current = GetEngine()->objectSelected->GetRot().ToEulerAngles();
			current += eulerRotate;
			GetEngine()->objectSelected->SetRot(current);
			break;
		}
		case SelectionMode::SCALE:
		{
			Core::Maths::Vec3 scale = s2 - s;
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

	Core::Maths::Vec3 WindowScene::GetCameraDirFromInput()
	{
		// Compute mouse pos to values between -1 and 1
		ImVec2 windowSize{ ImGui::GetContentRegionMax() };
		Core::Maths::Vec2 mouse = ImGui::GetMousePos();
		mouse -= ImGui::GetWindowPos();
		mouse.x -= windowSize.x / 2;
		mouse.x /= (windowSize.x / 2);
		mouse.y -= windowSize.y / 2;
		mouse.y /= (windowSize.y / 2);

		// If Mouse outside of window, return default vector
		if (mouse.x < -1 || mouse.x > 1 || mouse.y < -1 || mouse.y > 1)
			return Core::Maths::Vec3(0, 0, 0);

		Core::Maths::Vec3 dir = m_cam->GetPerspectiveDirection(mouse.x, mouse.y);
		dir.z *= -1; // Reverse direction for raycast use

		return dir;
	}

	void WindowScene::Tick()
	{
		DisplayScene();

		if (GetEngine()->isTestingRay)
		{
			GetEngine()->isTestingRay = false;
			if (!ImGuizmo::IsOver())
			{
				Core::Maths::Vec3 dir = GetCameraDirFromInput();
				if (dir.SquaredLength() != 0)
					GetEngine()->SelectObjectInScene(m_cam->GetPos(), dir);
			}
		}
	}
}
