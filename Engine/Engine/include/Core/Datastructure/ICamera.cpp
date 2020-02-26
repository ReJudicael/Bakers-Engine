#include "ICamera.h"
#include "RootObject.hpp"
#include "Debug.h"

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
