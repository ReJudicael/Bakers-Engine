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
	GetRoot()->AddCamera(this);
	m_fbo = GetRoot()->GetEngine()->CreateFBO(m_cameraWidth, m_cameraHeight, Core::Renderer::FBOType::CAMERA);
	m_fbo->userPtr = this;


	m_shadowShader = GetRoot()->GetEngine()->GetResourcesManager()->CreateShader("Shadow", "Resources\\Shaders\\ShadowShader.vert", "Resources\\Shaders\\ShadowShader.frag");
	
	glGenTextures(1, &m_depthTexture);
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		m_fbo->Size[2], m_fbo->Size[3], 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenFramebuffers(1, &m_depthStorage);
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthStorage);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool Core::Datastructure::ICamera::OnStart()
{
	return IComponent::OnStart();
}

Core::Datastructure::ICamera::~ICamera() noexcept
{
	if (GetRoot() == nullptr)
		return;
	GetRoot()->RemoveCamera(this);
}

void Core::Datastructure::ICamera::Draw(const std::list<Core::Datastructure::IRenderable*>& renderables)
{
	ZoneScoped
		ZoneText("Render of a camera", 22)
		TracyGpuZone("Rendering frame buffer")
	if (IsInit() && m_isActive && !IsDestroyed() && m_parent->IsActive())
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_depthStorage);
		glViewport(m_fbo->Size[0], m_fbo->Size[1], m_fbo->Size[2], m_fbo->Size[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto it{ renderables.begin() }; it != renderables.end(); ++it)
			(*it)->Draw(this->GetCameraMatrix(), this->GetPerspectiveMatrix(), m_shadowShader);

		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->FBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_depthTexture);

		for (auto it{ renderables.begin() }; it != renderables.end(); ++it)
			(*it)->Draw(this->GetCameraMatrix(), this->GetPerspectiveMatrix());

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
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

	SetRatio(width, height);
	
	m_fbo->Resize(width, height);

	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		m_fbo->Size[2], m_fbo->Size[3], 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Core::Datastructure::ICamera::OnDestroy()
{
	GetRoot()->RemoveCamera(this);
	GetRoot()->GetEngine()->DeleteFBO(m_fbo);
}

void	Core::Datastructure::ICamera::OnReset()
{
	IComponent::OnReset();
	GetRoot()->RemoveCamera(this);
	m_cameraHeight = 800;
	m_cameraWidth = 1280;

	if (m_fbo != nullptr)
	{
		GetRoot()->GetEngine()->DeleteFBO(m_fbo);
		m_fbo = nullptr;
	}
}
