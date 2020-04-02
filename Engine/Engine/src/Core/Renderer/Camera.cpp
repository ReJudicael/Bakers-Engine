#include <cmath>

#include "Camera.h"
#include "Object.hpp"

RTTR_PLUGIN_REGISTRATION
{
	registration::class_<Camera>("Camera")
		.constructor()
		.constructor<const float, const float, const float, const float>()
		.property("Ratio", &Camera::m_ratio)
		.property("FOV", &Camera::m_fov)
		.property("Near", &Camera::m_near)
		.property("Far", &Camera::m_far);
}

Core::Maths::Mat4 Camera::OnGenerateCamera()
{
	return m_parent->GetGlobalTRS().Inversed();
}

Core::Maths::Mat4 Camera::OnGeneratePerspective()
{
	return CreatePerspectiveMatrix(m_ratio, m_near, m_far, m_fov);
}

Camera::Camera() : 
	m_ratio{ 0 },
	m_fov{ 0 },
	m_near{ 0 },
	m_far{ 0 }
{
}

Camera::Camera(const float ratio, const float fov, const float near, const float far) :
	m_ratio{ ratio },
	m_fov{ fov },
	m_near{ near },
	m_far{ far }
{
}

void Camera::OnUpdate(float deltaTime)
{
	m_isCamUpdated = false;

	if (m_pRatio != m_ratio || m_pFov != m_fov
		|| m_pNear != m_near || m_pFar != m_far)
	{
		m_isPerspectiveUpdated = false;
		m_pRatio = m_ratio;
		m_pFov = m_fov;
		m_pNear = m_near;
		m_pFar = m_far;
	}
}

void Camera::OnStart()
{
	ICamera::OnStart();
	IUpdatable::OnStart();
}

void Camera::SetRatio(const float newRatio)
{
	m_ratio = newRatio;
	m_isPerspectiveUpdated = false;
}

Core::Maths::Mat4 Camera::CreatePerspectiveMatrix(const float ratio, const float near, const float far, const float fov)
{
	Core::Maths::Mat4 persp;

	float top = near * tan((M_PI * fov/180) / 2);
	float bot = -top;
	float right = top * ratio;
	float left = -right;

	persp.m_mat[0][0] = (2 * near) / (right - left);
	persp.m_mat[1][1] = (2 * near) / (top - bot);
	persp.m_mat[2][0] = (right + left) / (right - left);
	persp.m_mat[2][1] = (top + bot) / (top - bot);
	persp.m_mat[2][2] = - (far + near) / (far - near);
	persp.m_mat[2][3] = -1;
	persp.m_mat[3][2] = - (2 * near * far) / (far - near);

	return persp;
}

void Camera::OnCopy(void* toCopy) const
{
	ComponentBase::OnCopy(toCopy);
	ICamera::OnCopy(toCopy);
	IUpdatable::OnCopy(toCopy);

	Camera* copy = (Camera*)toCopy;

	copy->m_near = m_near;
	copy->m_far = m_far;
	copy->m_fov = m_fov;
	copy->m_ratio = m_ratio;

	copy->m_pNear = m_near;
	copy->m_pFar = m_far;
	copy->m_pFov = m_fov;
	copy->m_pRatio = m_ratio;
}

void	Camera::StartCopy(void*& copyTo) const
{
	copyTo = new Camera();
	OnCopy(copyTo);
}