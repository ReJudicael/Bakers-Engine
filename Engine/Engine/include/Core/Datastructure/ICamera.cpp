#include "ICamera.h"
#include "RootObject.hpp"
#include "Debug.h"

RTTR_REGISTRATION
{
	using namespace Core::Datastructure;
	registration::class_<ICamera>("ICamera")
		.method("GetCameraPerspective", &ICamera::GetPerspectiveMatrix)
		.method("GetCameraMatrix", &ICamera::GetCameraMatrix)
		.method("SetRatio", &ICamera::SetRatio)
		.property_readonly("front", &ICamera::m_front)
		.property_readonly("right", &ICamera::m_right);
}

const Core::Maths::Mat4& Core::Datastructure::ICamera::GetPerspectiveMatrix()
{
	if (!m_isPerspectiveUpdated)
	{
		ZoneScoped
			ZoneText("Generating perspective matrix", 30)
		m_perspective = OnGeneratePerspective();
		m_isPerspectiveUpdated = true;
	}
	return m_perspective;
}

const Core::Maths::Mat4& Core::Datastructure::ICamera::GetCameraMatrix()
{
	if (!m_isCamUpdated)
	{
		ZoneScoped
			ZoneText("Generating camera matrix", 25)
		m_cameraMatrix = OnGenerateCamera();
		m_front = Core::Maths::Vec3(m_parent->GetGlobalTRS() * Core::Maths::Vec4{ 0, 0, 1, 0 });
		m_right = Core::Maths::Vec3(m_parent->GetGlobalTRS() * Core::Maths::Vec4{ 1, 0, 0, 0 });
		m_isCamUpdated = true;
	}
	return m_cameraMatrix;
}

void Core::Datastructure::ICamera::OnStart()
{
	GetScene()->AddCamera(this);
}
