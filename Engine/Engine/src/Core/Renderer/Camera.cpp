#define _USE_MATH_DEFINES
#include <cmath>

#include "Camera.h"
#include "Object.hpp"

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

Camera::~Camera()
{
}

void	Camera::UpdateCamera()
{
	m_perspective = CreatePerspectiveMatrix(m_ratio, m_near, m_far, m_fov);
	m_cameraMatrix = m_parent->GetGlobalTRS().Inversed();
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