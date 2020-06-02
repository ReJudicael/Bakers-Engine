#include <cmath>

#include "Camera.h"
#include "Object.hpp"
#include "Maths.hpp"

RTTR_PLUGIN_REGISTRATION
{
	ZoneScopedN("Registering RTTR")
	registration::class_<Core::Renderer::CameraPerspective>("Perspective")
		.property("Width", &Core::Renderer::CameraPerspective::width)
		.property("Height", &Core::Renderer::CameraPerspective::height)
		.property("FOV", &Core::Renderer::CameraPerspective::fov)
		.property("Near", &Core::Renderer::CameraPerspective::near)
		.property("Far", &Core::Renderer::CameraPerspective::far);

	Core::Datastructure::RegisterDefaultClassConstructor<Core::Renderer::Camera>("Camera");
	registration::class_<Core::Renderer::Camera>("Camera")
		.constructor()
		.constructor<const float, const float, const float, const float, const float>();

	Core::Datastructure::RegisterClassPropertyGS<Core::Renderer::Camera>("Camera", "Perspective", &Core::Renderer::Camera::GetPerspData, &Core::Renderer::Camera::SetPerspData);
}

namespace Core::Renderer
{
	Core::Maths::Mat4 Camera::OnGenerateCamera()
	{
		return m_parent->GetGlobalTRS().Inversed();
	}

	bool Camera::IsCameraMatrixUpdated()
	{
		return !m_parent->IsTransformUpdated();
	}

	Core::Maths::Mat4 Camera::OnGeneratePerspective()
	{
		return CreatePerspectiveMatrix(m_persp.width, m_persp.height, m_persp.fov, m_persp.near, m_persp.far);
	}

	Camera::Camera()
	{
	}

	Camera::Camera(const float width, const float height, const float fov, const float near, const float far) : ComponentBase(), ICamera(), IUpdatable()
	{
		m_persp.width = width;
		m_persp.height = height;
		m_persp.fov = fov;
		m_persp.near = near;
		m_persp.far = far;
	}

	void Camera::OnUpdate(float deltaTime)
	{
	}

	bool Camera::OnStart()
	{
		ZoneScoped
		return ICamera::OnStart() && IUpdatable::OnStart();
	}

	Core::Maths::Vec3 Camera::GetPerspectiveDirection(const float ratioX, const float ratioY)
	{
		// Get camera forward and change it for camera ray use
		Core::Maths::Vec3 forward = Core::Maths::Vec3(0, 0, 1.f);

		float ratio = m_persp.width / m_persp.height;

		// Rotate forward towards fov limits with given ratios
		float radFOV = Core::Maths::ToRadiansf(m_persp.fov);
		Core::Maths::Quat RotateY = Core::Maths::Quat::AngleAxis(-radFOV * ratioX * 0.5f * ratio, { 0, 1.f, 0 });
		Core::Maths::Quat RotateX = Core::Maths::Quat::AngleAxis(-radFOV * ratioY * 0.5f, { 1.f, 0, 0 });
		Core::Maths::Quat FullRotation = RotateY * RotateX;

		Core::Maths::Vec3 localDirection = FullRotation.Rotate(forward);
		return m_parent->GetGlobalRot().Rotate(localDirection) * -1.f;
	}

	Core::Maths::Mat4 Camera::CreatePerspectiveMatrix(const float width, const float height, const float fov, const float near, const float far)
	{
		Core::Maths::Mat4 persp;

		float top = near * tanf((static_cast<float>(M_PI) * fov / 180.f) / 2.f);
		float bot = -top;
		float right = top * (width / height);
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

	Core::Maths::Mat4 Camera::CreateOrthographicMatrix(const float width, const float height, const float near, const float far)
	{
		Core::Maths::Mat4 ortho = Core::Maths::Mat4::Identity();

		float top{ height };
		float left{ 0 };
		float bot{ 0 };
		float right{ width };

		ortho(0, 0) = 4000 / (right - left);
		ortho(1, 1) = 4000 / (top - bot);
		ortho(2, 2) = -2 / (far - near);
		ortho(0, 3) = -((right + left) / (right - left));
		ortho(1, 3) = -((top + bot) / (top - bot));
		ortho(2, 3) = -((far + near) / (far - near));

		return ortho;
	}

	void Camera::SetRatio(const float width, const float height)
	{
		m_persp.width = width;
		m_persp.height = height;
		m_isPerspectiveUpdated = false;
	}

	void Camera::OnCopy(IComponent* toCopy) const
	{
		ZoneScoped
		ComponentBase::OnCopy(toCopy);
		ICamera::OnCopy(toCopy);
		IUpdatable::OnCopy(toCopy);

		Camera* copy = dynamic_cast<Camera*>(toCopy);

		copy->m_persp = m_persp;
	}

	void	Camera::StartCopy(IComponent*& copyTo) const
	{
		ZoneScoped
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
		m_isPerspectiveUpdated = false;
	}
}