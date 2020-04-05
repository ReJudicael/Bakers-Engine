#include <cmath>

#include "Camera.h"
#include "Object.hpp"

RTTR_PLUGIN_REGISTRATION
{
	registration::class_<Core::Renderer::CameraPerspective>("Perspective")
		.property("Ratio", &Core::Renderer::CameraPerspective::ratio)
		.property("FOV", &Core::Renderer::CameraPerspective::fov)
		.property("Near", &Core::Renderer::CameraPerspective::near)
		.property("Far", &Core::Renderer::CameraPerspective::far);

	registration::class_<Core::Renderer::Camera>("Camera")
		.constructor()
		.constructor<const float, const float, const float, const float>()
		.property("Perspective", &Core::Renderer::Camera::m_persp);
}

namespace Core::Renderer
{
	Core::Maths::Mat4 Camera::OnGenerateCamera()
	{
		return m_parent->GetGlobalTRS().Inversed();
	}

	Core::Maths::Mat4 Camera::OnGeneratePerspective()
	{
		return CreatePerspectiveMatrix(m_persp.ratio, m_persp.fov, m_persp.near, m_persp.far);
	}

	Camera::Camera()
	{
	}

	Camera::Camera(const float ratio, const float fov, const float near, const float far) : ComponentBase(), ICamera(), IUpdatable()
	{
		m_persp.ratio = m_prevPersp.ratio = ratio;
		m_persp.fov = m_prevPersp.ratio = fov;
		m_persp.near = m_prevPersp.ratio = near;
		m_persp.far = m_prevPersp.ratio = far;
	}

	void Camera::OnUpdate(float deltaTime)
	{
		m_isCamUpdated = false;

		if (PerspectiveNeedUpdate())
		{
			m_isPerspectiveUpdated = false;
			m_prevPersp = m_persp;
		}
	}

	void Camera::OnStart()
	{
		ICamera::OnStart();
		IUpdatable::OnStart();
	}

	Core::Maths::Mat4 Camera::CreatePerspectiveMatrix(const float ratio, const float fov, const float near, const float far)
	{
		Core::Maths::Mat4 persp;

		float top = near * tanf((M_PI * fov / 180) / 2);
		float bot = -top;
		float right = top * ratio;
		float left = -right;

		persp.m_mat[0][0] = (2 * near) / (right - left);
		persp.m_mat[1][1] = (2 * near) / (top - bot);
		persp.m_mat[2][0] = (right + left) / (right - left);
		persp.m_mat[2][1] = (top + bot) / (top - bot);
		persp.m_mat[2][2] = -(far + near) / (far - near);
		persp.m_mat[2][3] = -1;
		persp.m_mat[3][2] = -(2 * near * far) / (far - near);

		return persp;
	}

	void Camera::SetRatio(const float newRatio)
	{
		m_persp.ratio = newRatio;
		m_prevPersp.ratio = newRatio;
		m_isPerspectiveUpdated = false;
	}

	bool Camera::PerspectiveNeedUpdate() const noexcept
	{
		bool result = m_prevPersp.ratio != m_persp.ratio;
		result |= m_prevPersp.fov != m_persp.fov;
		result |= m_prevPersp.near != m_persp.near;
		result |= m_prevPersp.far != m_persp.far;

		return result;
	}

	void Camera::OnCopy(IComponent* toCopy) const
	{
		ComponentBase::OnCopy(toCopy);
		ICamera::OnCopy(toCopy);
		IUpdatable::OnCopy(toCopy);

		Camera* copy = dynamic_cast<Camera*>(toCopy);

		copy->m_persp = m_persp;
		copy->m_prevPersp = m_prevPersp;
	}

	void	Camera::StartCopy(IComponent*& copyTo) const
	{
		copyTo = new Camera();
		OnCopy(copyTo);
	}

	void	Camera::OnDestroy()
	{
		ComponentBase::OnDestroy();
		ICamera::OnDestroy();
		IUpdatable::OnDestroy();
	}

	void	Camera::OnReset()
	{
		ComponentBase::OnReset();
		ICamera::OnReset();
		IUpdatable::OnReset();
	}
}