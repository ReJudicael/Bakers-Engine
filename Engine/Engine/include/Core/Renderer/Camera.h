#pragma once

#include "ComponentBase.h"
#include "ICamera.h"

class Camera : public Core::Datastructure::ComponentBase, public virtual Core::Datastructure::ICamera
{
private:
	float m_ratio;
	float m_fov;
	float m_near;
	float m_far;

public:
	/**
	 * Default Constructor
	 */
	Camera();

	Camera(const float ratio, const float fov, const float near, const float far);

	/**
	 * Destructor
	 */
	~Camera();

	virtual void OnStart() override {};
	virtual void OnDestroy() override {};

	/**
	 * Update Camera matrices based on its current values
	 */
	void UpdateCamera();

	/**
	 * Get reverse TRS matrix of the camera
	 * @return Reverse TRS camera matrix
	 */
	Core::Maths::Mat4	GetCameraMatrix() noexcept;

	/**
	 * Get perspective matrix of the camera
	 * @return Stored perspective matrix
	 */
	Core::Maths::Mat4	GetPerspectiveMatrix() noexcept;

	/**
	 * Create Perspective Matrix
	 * @param ratio: Ratio (width / height) of the window
	 * @param near: Distance where objects start to be rendered
	 * @param far: Distance where objects are no longer rendered
	 * @param fov: Angle of the field of view of the camera
	 * @return Perspective matrix
	 */
	static Core::Maths::Mat4 CreatePerspectiveMatrix(const float ratio, const float near, const float far, const float fov);
};

inline Core::Maths::Mat4	Camera::GetCameraMatrix() noexcept
{
	return m_cameraMatrix;
}

inline Core::Maths::Mat4	Camera::GetPerspectiveMatrix() noexcept
{
	return m_perspective;
}
