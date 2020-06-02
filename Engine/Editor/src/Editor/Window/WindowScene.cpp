#include "WindowScene.h"
#include "ICamera.h"
#include "Framebuffer.h"
#include "EditorEngine.h"
#include "RootObject.hpp"
#include "Object.hpp"
#include "ImGuizmo.h"
#include "Maths.hpp"

#include "IconsFontAwesome5.h"

namespace Editor::Window
{
	WindowScene::WindowScene(Canvas* canvas, bool visible) :
		AWindow{ canvas, ICON_FA_BORDER_ALL "  Scene", visible }
	{
		m_flags |= ImGuiWindowFlags_NoScrollbar;

		m_cam = new Datastructure::EditorCamera();
		GetEngine()->m_root->AddComponent(m_cam);

		m_cube.array[14] = 10;

		m_model.push_back(GetEngine()->GetResourcesManager()->GetModel("Cube"));
		m_model.push_back(GetEngine()->GetResourcesManager()->GetModel("Sphere"));
		m_model.push_back(GetEngine()->GetResourcesManager()->GetModel("Capsule"));

		m_shader = GetEngine()->GetResourcesManager()->GetShader("Wireframe");
	}

	void WindowScene::PushWindowStyle()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });
	}

	void WindowScene::PopWindowStyle()
	{
		ImGui::PopStyleVar(1);
	}

	void WindowScene::DrawAllColliders()
	{
		std::list<Core::Physics::Collider*> box = GetEngine()->m_boxCollider;
		for (auto it = box.begin(); it != box.end(); ++it)
			(*it)->DrawCollider(m_cam, m_shader, m_model[0]);

		std::list<Core::Physics::Collider*> sphere = GetEngine()->m_sphereCollider;
		for (auto it = sphere.begin(); it != sphere.end(); ++it)
			(*it)->DrawCollider(m_cam, m_shader, m_model[1]);

		std::list<Core::Physics::Collider*> capsule = GetEngine()->m_capsuleCollider;
		for (auto it = capsule.begin(); it != capsule.end(); ++it)
			(*it)->DrawCollider(m_cam, m_shader, m_model[2]);
	}

	void WindowScene::DrawGuizmo(ImVec2 windowSize)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowSize.x, windowSize.y);

		CheckGizmoUse();
		GizmoViewManipulateResult();
		GizmoManipulateResult();
	}

	void WindowScene::DrawOptions()
	{
		ImGui::PushStyleColor(ImGuiCol_Button, GImGui->Style.Colors[ImGuiCol_FrameBg]);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 4.f, 4.f });
		bool isClicked = ImGui::BeginComboButton(ICON_FA_COG "  Show Gizmos", { 125.f, 0.f });
		ImGui::PopStyleColor();

		if (isClicked)
		{
			ImGui::MenuItem("Nav Mesh", (const char*)0, &m_showNavMesh);
			ImGui::MenuItem("Colliders", (const char*)0, &m_showColliders);
			ImGui::EndCombo();
		}

		ImGui::PopStyleVar();
	}

	void WindowScene::DisplayScene()
	{
		if (m_cam)
		{
			ImVec2 windowSize{ ImGui::GetContentRegionAvail() };
			Core::Renderer::Framebuffer* fbo{ m_cam->GetFBO() };

			{
				GLint PreviousFramebuffer;
				glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &PreviousFramebuffer);
				glBindFramebuffer(GL_FRAMEBUFFER, fbo->FBO);
				glViewport(fbo->Size[0], fbo->Size[1], fbo->Size[2], fbo->Size[3]);
				if (m_showNavMesh)
					GetEngine()->GetNavMesh()->DrawNavMesh(m_cam);
				if (m_showColliders)
					DrawAllColliders();
				glBindFramebuffer(GL_FRAMEBUFFER, PreviousFramebuffer);
			}

			float cursorY{ ImGui::GetCursorPosY() };
			ImGui::ImageUV(fbo->ColorTexture, windowSize);
			ImGui::SetCursorPos({ ImGui::GetWindowContentRegionWidth() - 128.f, cursorY + GImGui->Style.ItemSpacing.y });
			DrawOptions();

			if (ImGui::IsWindowHovered())
				m_cam->Update(ImGui::GetIO().DeltaTime, GetEngine()->m_editorInput);
			else if (GetEngine()->m_editorInput->IsCursorHidden())
				GetEngine()->m_editorInput->SetMouseAppearance(Core::SystemManagement::ECursorAppearance::DEFAULT);

			if (fbo->Size[2] != windowSize.x || fbo->Size[3] != windowSize.y)
				m_cam->Resize(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));

			if (GetEngine()->objectSelected && GetEngine()->operation != SelectionMode::MOVEMENT)
				DrawGuizmo(windowSize);
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

		if (ImGui::IsWindowHovered())
			ImGuizmo::Enable(true);
		else
			ImGuizmo::Enable(false);
	}

	void WindowScene::GizmoViewManipulateResult()
	{
		// Save transform values before gizmo manipulate
		Core::Maths::Vec3 t, r, s;
		ImGuizmo::DecomposeMatrixToComponents(m_cube.array, t.xyz, r.xyz, s.xyz);

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
			r2.x = Core::Maths::ToRadiansf(-r2.x);
			r2.y = Core::Maths::ToRadiansf(-r2.y);
			r2.z = Core::Maths::ToRadiansf(r2.z);
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
			eulerRotate.x = Core::Maths::ToRadiansf(eulerRotate.x);
			eulerRotate.y = Core::Maths::ToRadiansf(eulerRotate.y);
			eulerRotate.z = Core::Maths::ToRadiansf(eulerRotate.z);
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
		dir.Normalize();

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
