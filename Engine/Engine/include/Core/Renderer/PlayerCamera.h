#pragma once

#include "Camera.h"
#include "Vec2.hpp"

/**
 * Camera controllable through WASD and mouse inputs
 */
class PlayerCamera : public Camera
{
private:
	// Movement through WASD keys
	bool	m_isRunning{ false };
	float	m_speed{ 1 };
	float	m_runningSpeed{ 5 };
	float	m_minMoveLength{ 0.001 };
	float	m_moveLerpSpeed{ 0.5 };
	Core::Maths::Vec3 m_movement;

	// Rotation through mouse movements
	bool	m_isRotating{ false };
	bool	m_isMouseSet{ false };
	float	m_angularSpeed{ 0.15f };
	float	m_yaw{ 0.f };
	float	m_pitch{ 0.f };
	float	m_minAngularLength{ 0.001 };
	float	m_angularLerpSpeed{ 0.5 };
	Core::Maths::Vec3 m_angularMovement;
	Core::Maths::Vec2	m_mousePos;

public:
	/**
	 * Default Constructor
	 */
	PlayerCamera();

	/**
	 * Constructor initializing camera variables
	 */
	PlayerCamera(const float ratio, const float fov, const float near, const float far);

	/**
	 * Compute keyboard and mouse interactions when right mouse button is pressed
	 */
	void	MoveWithInput();

	/**
	 * Compute translation with WASD keys
	 */
	void	ComputeTranslation();

	/**
	 * Compute rotation with mouse movements
	 */
	void	ComputeRotation();

	/**
	 * Update translation and rotation
	 * @param deltaTime: Time elapsed between two frames
	 */
	virtual void OnUpdate(float deltaTime) override;

	/**
	 * Update translation with stored movement vector
	 * @param deltaTime: Time elapsed between two frames
	 */
	void	UpdatePosition(float deltaTime);

	/**
	 * Update rotation with stored rotation vector
	 * @param deltaTime: Time elapsed between two frames
	 */
	void	UpdateRotation(float deltaTime);

	/**
	 * Update movement vector (change applied by OnUpdate)
	 * @param move: New value for stored movement vector
	 */
	void	Move(Core::Maths::Vec3 move);
	
	/**
	 * Update rotation vector (change applied by OnUpdate)
	 * @param move: New value for stored angular movement vector
	 */
	void	Rotate(Core::Maths::Vec3 move);
};

