#include "ICamera.h"
#include "RootObject.hpp"

const Core::Maths::Mat4& Core::Datastructure::ICamera::GetPerspectiveMatrix()
{
	
	if (!m_isPerspectiveUpdated)
	{
		m_perspective = OnGeneratePerspective();
		m_isPerspectiveUpdated = true;
	}
	return m_perspective;
}

const Core::Maths::Mat4& Core::Datastructure::ICamera::GetCameraMatrix()
{
	if (!m_isCamUpdated)
	{
		m_cameraMatrix = OnGenerateCamera();
		m_isCamUpdated = true;
	}
	return m_cameraMatrix;
}

void Core::Datastructure::ICamera::OnStart()
{
	GetScene()->AddCamera(this);
}
