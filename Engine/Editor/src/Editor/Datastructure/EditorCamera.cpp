#include "EditorCamera.h"
#include "Camera.h"
#include "RootObject.hpp"
#include "EngineCore.h"
#include "EditorEngine.h"
#include "Maths.hpp"

namespace Editor::Datastructure
{
	EditorCamera::EditorCamera() : m_transform{ Core::Datastructure::Transform({0, 1, 0}) }
	{
	}

	void EditorCamera::SetRatio(const float newRatio)
	{
		m_persp.ratio = newRatio;
		m_isPerspectiveUpdated = false;
	}

	Core::Renderer::Framebuffer* EditorCamera::GetFBO()
	{
		return m_fbo;
	}
	
	Core::Maths::Vec3 EditorCamera::GetPerspectiveDirection(const float ratioX, const float ratioY)
	{
		constexpr Core::Maths::Quat	forQuat{ 0, 0, 0, 1 };
		float radFOV = Core::Maths::ToRadians(m_persp.fov);
		Core::Maths::Vec3 forward = (m_transform.GetGlobalRot() * forQuat * m_transform.GetGlobalRot().Inversed()).GetVec();
		Core::Maths::Quat RotateY = Core::Maths::Quat::AngleAxis(radFOV * ratioX, m_parent->Up());
		Core::Maths::Quat RotateX = Core::Maths::Quat::AngleAxis(radFOV * ratioY, m_parent->Right());
		Core::Maths::Quat FullRotation = RotateX * RotateY;

		return FullRotation.Rotate(forward);
	}

	void EditorCamera::SetRot(const Core::Maths::Vec3& v)
	{
		m_transform.SetLocalRot(v);
		m_isCamUpdated = false;
	}

	void EditorCamera::SetPos(const Core::Maths::Vec3& v)
	{
		m_transform.SetLocalPos(v);
		m_isCamUpdated = false;
	}

	const Core::Maths::Vec3& EditorCamera::GetPos()
	{
		return m_transform.GetLocalPos();
	}

	void EditorCamera::OnInit()
	{
		ComponentBase::OnInit();
		ICamera::OnInit();
		m_fbo->type = Core::Renderer::FBOType::CUSTOM;
	}
	
	Core::Maths::Mat4 EditorCamera::OnGenerateCamera()
	{
		return m_transform.GetLocalTrs().Inversed();
	}
	
	Core::Maths::Mat4 EditorCamera::OnGeneratePerspective()
	{
		return Core::Renderer::Camera::CreatePerspectiveMatrix(m_persp.ratio, m_persp.fov, m_persp.near, m_persp.far);
	}
	
	void EditorCamera::StartCopy(IComponent*& copyTo) const
	{
		copyTo = new EditorCamera();
		OnCopy(copyTo);
	}
	
	void EditorCamera::OnCopy(IComponent* copyTo) const
	{
		ComponentBase::OnCopy(copyTo);
		ICamera::OnCopy(copyTo);
	}
	
	void EditorCamera::OnDestroy()
	{
		ComponentBase::OnDestroy();
		ICamera::OnDestroy();

		GetRoot()->GetEngine()->DeleteFBO(m_fbo);
	}
	
	void EditorCamera::OnReset()
	{
		ComponentBase::OnReset();
		ICamera::OnReset();

		m_persp = CameraPerspective();
		m_isPerspectiveUpdated = false;
		m_transform = Core::Datastructure::Transform({0, 1, 0});
	}

	void EditorCamera::MoveWithInput()
	{
		if (Input()->IsMouseButtonDown(EMouseButton::RIGHT))
		{ 
			Input()->SetMouseAppearance(Core::SystemManagement::ECursorAppearance::INVISIBLE);
			ComputeInputTranslation();
			ComputeRotation();
			
		}
		else if (IsUsingMouseTranslation())
		{
			ComputeMouseTranslation();
		}
		else
		{
			Input()->SetMouseAppearance(Core::SystemManagement::ECursorAppearance::DEFAULT);
			m_mousePos = Input()->GetMousePos();
			m_isMouseSet = false;
			return;
		}
	}

	bool EditorCamera::IsUsingMouseTranslation()
	{
		if (Input()->IsMouseButtonDown(EMouseButton::MIDDLE))
			return true;

		Editor::EditorEngine* e = dynamic_cast<Editor::EditorEngine*>(GetRoot()->GetEngine());
		bool result = (e->operation == SelectionMode::MOVEMENT);
		return (result && Input()->IsMouseButtonDown(EMouseButton::LEFT));
	}

	void EditorCamera::ComputeInputTranslation()
	{
		Core::Maths::Vec3 move = { 0, 0, 0 };
		constexpr Core::Maths::Vec3 forward{ 0, 0, 1 };
		constexpr Core::Maths::Quat	forQuat{ 0, forward };
		constexpr Core::Maths::Vec3 right{ 1, 0, 0 };
		constexpr Core::Maths::Quat	rightQuat{ 0, right };
		constexpr Core::Maths::Vec3 up{ 0, 1, 0 };
		constexpr Core::Maths::Quat upQuat{ 0, up };
		
		if (Input()->IsKeyDown(EKey::W))
			move += (m_transform.GetLocalRot() * forQuat * m_transform.GetLocalRot().Inversed()).GetVec() * -1;
		if (Input()->IsKeyDown(EKey::A))
			move += (m_transform.GetLocalRot() * rightQuat * m_transform.GetLocalRot().Inversed()).GetVec() * -1;
		if (Input()->IsKeyDown(EKey::D))
			move += (m_transform.GetLocalRot() * rightQuat * m_transform.GetLocalRot().Inversed()).GetVec();
		if (Input()->IsKeyDown(EKey::S))
			move += (m_transform.GetLocalRot() * forQuat * m_transform.GetLocalRot().Inversed()).GetVec();
		if (Input()->IsKeyDown(EKey::Q))
			move += (m_transform.GetLocalRot() * upQuat * m_transform.GetLocalRot().Inversed()).GetVec() * -1;
		if (Input()->IsKeyDown(EKey::E))
			move += (m_transform.GetLocalRot() * upQuat * m_transform.GetLocalRot().Inversed()).GetVec();

		if (move.SquaredLength() > 0)
			Move(move);
	}

	void EditorCamera::ComputeMouseTranslation()
	{
		Core::Maths::Vec2 newPos = Input()->GetMousePos();
		Core::Maths::Vec2 mouseMove = m_mousePos - newPos;

		Core::Maths::Vec3 move = { 0, 0, 0 };
		constexpr Core::Maths::Vec3 right{ 1, 0, 0 };
		constexpr Core::Maths::Quat	rightQuat{ 0, right };
		constexpr Core::Maths::Vec3 up{ 0, 1, 0 };
		constexpr Core::Maths::Quat upQuat{ 0, up };

		Core::Maths::Vec3 rightMovement = (m_transform.GetLocalRot() * rightQuat * m_transform.GetLocalRot().Inversed()).GetVec();
		Core::Maths::Vec3 upMovement = (m_transform.GetLocalRot() * upQuat * m_transform.GetLocalRot().Inversed()).GetVec();

		move = rightMovement * mouseMove.x + upMovement * mouseMove.y * -1;

		if (move.SquaredLength() > 0)
			Move(move);

		m_mousePos = newPos;
	}

	void EditorCamera::ComputeRotation()
	{
		Core::Maths::Vec2 newPos = Input()->GetMousePos();
		Core::Maths::Vec2 mouseMove = m_mousePos - newPos;

		if (mouseMove.SquaredLength() > 0)
		{
			if (m_isMouseSet)
				Rotate({ mouseMove.y, mouseMove.x, 0 });
		}

		Input()->SetMousePos(m_mousePos);
		m_isMouseSet = true;
	}

	void EditorCamera::Update(float deltaTime)
	{
		MoveWithInput();

		UpdatePosition(deltaTime);
		UpdateRotation(deltaTime);
	}

	void EditorCamera::UpdatePosition(float deltaTime)
	{
		float length = m_movement.SquaredLength();

		if (length == 0) // Do not update transform if no movement has been made
			return;

		m_transform.Translate(m_movement * m_speed * deltaTime);

		// Decrease movement
		m_movement *= 0.5f;

		// Stop movement if it is too small
		if (length < 0.001f)
			m_movement = { 0, 0, 0 };
	}

	void EditorCamera::UpdateRotation(float deltaTime)
	{
		if (m_isRotating)
		{
			// Increase current rotation with new one
			m_pitch += 0.5f * m_angularMovement.x * deltaTime;
			m_yaw += 0.5f * m_angularMovement.y * deltaTime;

			// Create euler angles vector3 with pitch and yaw rotation
			Core::Maths::Vec3 newRotation(m_pitch, m_yaw, 0);
			Core::Maths::Quat q(newRotation);
			m_transform.SetLocalRot(q);
		}

		// Decrease rotation
		m_angularMovement *= 0.5f;

		// Stop rotation if too small
		if (m_angularMovement.SquaredLength() < 0.001f)
		{
			m_angularMovement = { 0, 0, 0 };
			m_isRotating = false;
		}
	}

	void EditorCamera::Move(Core::Maths::Vec3 move)
	{
		m_movement = move.Normalized();
	}

	void EditorCamera::Rotate(Core::Maths::Vec3 move)
	{
		m_angularMovement = move;
		m_isRotating = true;
	}
}
