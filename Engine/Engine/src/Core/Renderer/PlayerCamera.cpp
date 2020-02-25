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
		return;
	
	Core::Maths::Vec3 move = { 0, 0, 0 };

	if (Input()->IsKeyDown(EKey::W))
		move += { 0, 0, -1 };
	if (Input()->IsKeyDown(EKey::A))
		move += { -1, 0, 0 };
	if (Input()->IsKeyDown(EKey::D))
		move += { 1, 0, 0 };
	if (Input()->IsKeyDown(EKey::S))
		move += { 0, 0, 1 };

	if (move.SquaredLength() > 0)
		Move(move);
}

void PlayerCamera::OnUpdate(float deltaTime)
{
	MoveWithInput();

	Camera::OnUpdate(deltaTime);

	m_parent->Translate(m_movement * m_speed * deltaTime);
	m_movement *= 0.1;

	Core::Maths::Quaternion q(m_angularMovement * m_angularSpeed * deltaTime);
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
