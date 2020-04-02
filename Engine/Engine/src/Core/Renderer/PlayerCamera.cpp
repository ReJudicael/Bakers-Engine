#include <iostream>

#include "PlayerCamera.h"
#include "Object.hpp"
#include "InputSystem.hpp"

RTTR_PLUGIN_REGISTRATION
{
	registration::class_<PlayerCamera>("PlayerCamera")
		.constructor()
		.constructor<const float, const float, const float, const float>()
		.property("Is running", &PlayerCamera::m_isRunning)
		.property("Speed", &PlayerCamera::m_speed)
		.property("Running speed", &PlayerCamera::m_runningSpeed)
		.property("Min move length", &PlayerCamera::m_minMoveLength)
		.property("Move lerp speed", &PlayerCamera::m_moveLerpSpeed);
}

PlayerCamera::PlayerCamera() : Camera()
{
}

PlayerCamera::PlayerCamera(const float ratio, const float fov, const float near, const float far) : Camera(ratio, fov, near, far)
{
}

void PlayerCamera::MoveWithInput()
{
	if (Input()->IsMouseButtonUnused(EMouseButton::RIGHT))
	{
		Input()->SetMouseAppearance(Core::SystemManagement::ECursorAppearance::DEFAULT);
		m_mousePos = Input()->GetMousePos();
		m_isMouseSet = false;
		return;
	}
	
	Input()->SetMouseAppearance(Core::SystemManagement::ECursorAppearance::INVISIBLE);
	ComputeTranslation();
	ComputeRotation();
}

void PlayerCamera::ComputeTranslation()
{
	Core::Maths::Vec3 move = { 0, 0, 0 };

	if (Input()->IsKeyDown(EKey::W))
		move += m_parent->Forward() * -1;
	if (Input()->IsKeyDown(EKey::A))
		move += m_parent->Right() * -1;
	if (Input()->IsKeyDown(EKey::D))
		move += m_parent->Right();
	if (Input()->IsKeyDown(EKey::S))
		move += m_parent->Forward();

	if (Input()->IsKeyDown(EKey::LEFT_SHIFT))
		m_isRunning = true;
	else
		m_isRunning = false;

	if (move.SquaredLength() > 0)
		Move(move);
}

void PlayerCamera::ComputeRotation()
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

void PlayerCamera::OnUpdate(float deltaTime)
{
	MoveWithInput();

	Camera::OnUpdate(deltaTime);

	UpdatePosition(deltaTime);
	UpdateRotation(deltaTime);	
}

void PlayerCamera::UpdatePosition(float deltaTime)
{
	float length = m_movement.SquaredLength();

	if (length == 0) // Do not update transform if no movement has been made
		return;

	if (m_isRunning)
		m_parent->Translate(m_movement * m_runningSpeed * deltaTime);
	else
		m_parent->Translate(m_movement * m_speed * deltaTime);

	// Decrease movement
	m_movement *= m_moveLerpSpeed;

	// Stop movement if it is too small
	if (length < m_minMoveLength)
		m_movement = { 0, 0, 0 };
}

void PlayerCamera::UpdateRotation(float deltaTime)
{
	if (m_isRotating)
	{
		// Increase current rotation with new one
		m_pitch += m_angularSpeed * m_angularMovement.x * deltaTime;
		m_yaw += m_angularSpeed * m_angularMovement.y * deltaTime;

		// Create euler angles vector3 with pitch and yaw rotation
		Core::Maths::Vec3 newRotation(m_pitch, m_yaw, 0);
		Core::Maths::Quat q(newRotation);
		m_parent->SetRot(q);
	}

	// Decrease rotation
	m_angularMovement *= m_angularLerpSpeed;

	// Stop rotation if too small
	if (m_angularMovement.SquaredLength() < m_minAngularLength)
	{
		m_angularMovement = { 0, 0, 0 };
		m_isRotating = false;
	}
}

void PlayerCamera::Move(Core::Maths::Vec3 move)
{
	m_movement = move.Normalized();
}

void PlayerCamera::Rotate(Core::Maths::Vec3 move)
{
	m_angularMovement = move;
	m_isRotating = true;
}

void PlayerCamera::OnCopy(void* copyTo) const
{
	Camera::OnCopy(copyTo);
	PlayerCamera* copy{ (PlayerCamera*)copyTo };

	copy->m_speed = m_speed;
	copy->m_runningSpeed = m_runningSpeed;
	copy->m_minMoveLength = m_minMoveLength;
	copy->m_moveLerpSpeed = m_moveLerpSpeed;
	copy->m_movement = m_movement;
	copy->m_isRotating = m_isRotating;
	copy->m_isMouseSet = m_isMouseSet;
	copy->m_angularSpeed = m_angularSpeed;
	copy->m_pitch = m_pitch;
	copy->m_yaw = m_yaw;
	copy->m_minAngularLength = m_minAngularLength;
	copy->m_angularLerpSpeed = m_angularLerpSpeed;
	copy->m_angularMovement = m_angularMovement;
	copy->m_mousePos = m_mousePos;
}

void PlayerCamera::StartCopy(void*& copyTo) const
{
	copyTo = new PlayerCamera();
	OnCopy(copyTo);
}
