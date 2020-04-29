#include "ICamera.h"
#include "RootObject.hpp"
#include "EngineCore.h"
#include "CoreMinimal.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace Core::Datastructure;
	registration::class_<ICamera>("ICamera")
		.method("GetCameraPerspective", &ICamera::GetPerspectiveMatrix)
		.method("GetCameraMatrix", &ICamera::GetCameraMatrix)
		.method("SetRatio", &ICamera::SetRatio);
}

const Core::Maths::Mat4& Core::Datastructure::ICamera::GetPerspectiveMatrix()
{
	if (!IsPerspectiveMatrixUpdated())
	{
		ZoneScoped
			ZoneText("Generating perspective matrix", 30)
		m_perspective = OnGeneratePerspective();
		PerspectiveMatrixUpdated();
	}
	return m_perspective;
}

const Core::Maths::Mat4& Core::Datastructure::ICamera::GetCameraMatrix()
{
	if (!IsCameraMatrixUpdated())
	{
		ZoneScoped
			ZoneText("Generating camera matrix", 25)
		m_cameraMatrix = OnGenerateCamera();
		CameraMatrixUpdated();
	}
	return m_cameraMatrix;
}

void Core::Datastructure::ICamera::OnInit()
{
	GetScene()->AddCamera(this);
	m_fbo = GetScene()->GetEngine()->CreateFBO(m_cameraWidth, m_cameraHeight, Core::Renderer::FBOType::CAMERA);
	m_fbo->userPtr = this;
}

bool Core::Datastructure::ICamera::OnStart()
{
	return IComponent::OnStart();
}

Core::Datastructure::ICamera::~ICamera() noexcept
{
	if (GetScene() == nullptr)
		return;
	GetScene()->RemoveCamera(this);
}

void Core::Datastructure::ICamera::Draw(const std::list<Core::Datastructure::IRenderable*>& renderables)
{
	ZoneScoped
		ZoneText("Render of a camera", 22)
		TracyGpuZone("Rendering frame buffer")
	if (IsInit() && m_isActive && !IsDestroyed() && m_parent->IsActive())
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->FBO);
		glViewport(m_fbo->Size[0], m_fbo->Size[1], m_fbo->Size[2], m_fbo->Size[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto it{ renderables.begin() }; it != renderables.end(); ++it)
			(*it)->Draw(this);
	}
}

void Core::Datastructure::ICamera::OnCopy(IComponent* copyTo) const
{
	IComponent::OnCopy(copyTo);
	ICamera* copy{ dynamic_cast<Core::Datastructure::ICamera*>(copyTo) };

	copy->m_cameraHeight = m_cameraHeight;
	copy->m_cameraWidth = m_cameraWidth;
}

void Core::Datastructure::ICamera::Resize(unsigned width, unsigned height)
{
	m_cameraWidth = width;
	m_cameraHeight = height;

	SetRatio(width / (float)height);
	
	m_fbo->Resize(width, height);
}

void Core::Datastructure::ICamera::OnDestroy()
{
	GetScene()->RemoveCamera(this);
	GetScene()->GetEngine()->DeleteFBO(m_fbo);
}

void	Core::Datastructure::ICamera::OnReset()
{
	IComponent::OnReset();
	GetScene()->RemoveCamera(this);
	m_cameraHeight = 800;
	m_cameraWidth = 1280;

	if (m_fbo != nullptr)
	{
		GetScene()->GetEngine()->DeleteFBO(m_fbo);
		m_fbo = nullptr;
	}
}
