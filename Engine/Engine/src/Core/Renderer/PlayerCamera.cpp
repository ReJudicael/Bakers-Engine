#include <iostream>

#include "PlayerCamera.h"
#include "Object.hpp"


PlayerCamera::PlayerCamera() : Camera()
{
}

PlayerCamera::PlayerCamera(const float ratio, const float fov, const float near, const float far) : Camera(ratio, fov, near, far)
{
}

void PlayerCamera::MoveWithInput()
{
	if (Input()->GetMouseButtonState(EMouseButton::RIGHT) == EStateMouseButton::UNUSED)
	{
		m_mousePos = 0;
		return;
	}
	
	Core::Maths::Vec3 move = { 0, 0, 0 };

	if (Input()->IsKeyDown(EKey::W))
		move += m_front * -1;
	if (Input()->IsKeyDown(EKey::A))
		move += m_right * -1;
	if (Input()->IsKeyDown(EKey::D))
		move += m_right;
	if (Input()->IsKeyDown(EKey::S))
		move += m_front;

	if (move.SquaredLength() > 0)
		Move(move);

	Core::Maths::Vec2 newPos = Input()->GetMousePos();
	Core::Maths::Vec2 mouseMove = m_mousePos - newPos;
	
	if (mouseMove.SquaredLength() > 0)
	{
		if (Input()->GetMouseButtonState(EMouseButton::RIGHT) != EStateMouseButton::PRESS)
			Rotate({ mouseMove.y, mouseMove.x, 0 });
	}

	m_mousePos = newPos;
}

void PlayerCamera::OnUpdate(float deltaTime)
{
	MoveWithInput();

	Camera::OnUpdate(deltaTime);

	m_parent->Translate(m_movement * m_speed * deltaTime);
	m_movement *= 0.1;

	Core::Maths::Vec3 v1 = (Core::Maths::Vec3)(m_parent->GetGlobalTRS() * Core::Maths::Vec4(1, 0, 0, 0));
	Core::Maths::Vec3 v2 = (Core::Maths::Vec3)(m_parent->GetGlobalTRS() * Core::Maths::Vec4(0, 1, 0, 0));
	
	Core::Maths::Quat q1 = Core::Maths::Quaternion::AngleAxis(m_angularMovement.x * m_angularSpeed * deltaTime, v1);
	Core::Maths::Quat q2 = Core::Maths::Quaternion::AngleAxis(m_angularMovement.y * m_angularSpeed * deltaTime, v2);
	Core::Maths::Quat q = q2 * q1;
	m_parent->Rotate(q);
	
	m_angularMovement *= 0.1;

	if (m_movement.SquaredLength() > 0.001)
		m_movement = { 0, 0, 0 };
	if (m_angularMovement.SquaredLength() > 0.001)
		m_angularMovement = { 0, 0, 0 };
}

void PlayerCamera::Move(Core::Maths::Vec3 move)
{
	m_movement = move.Normalized();
}

void PlayerCamera::Rotate(Core::Maths::Vec3 move)
{
	m_angularMovement = move.Normalized();
}
