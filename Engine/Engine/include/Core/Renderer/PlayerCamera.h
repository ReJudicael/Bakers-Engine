#pragma once

#include "Camera.h"

class PlayerCamera : public Camera
{
private:
	Core::Maths::Vec3 m_movement;
	float	m_speed = 1;

	Core::Maths::Vec3 m_angularMovement;
	float	m_angularSpeed = 0.1;

public:
	/**
	 * Default Constructor
	 */
	PlayerCamera();

	/**
	 * Constructor initializing camera variables
	 */
	PlayerCamera(const float ratio, const float fov, const float near, const float far);

	virtual void OnUpdate(float deltaTime) override;

	void	Move(Core::Maths::Vec3 move);
	void	Rotate(Core::Maths::Vec3 move);
};

