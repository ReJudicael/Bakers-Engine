#include <iostream>

#include "PlayerCamera.h"
#include "Object.hpp"


PlayerCamera::PlayerCamera() : Camera()
{
}

PlayerCamera::PlayerCamera(const float ratio, const float fov, const float near, const float far) : Camera(ratio, fov, near, far)
{
}

void PlayerCamera::OnUpdate(float deltaTime)
{
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
