#include "ICamera.h"
#include "RootObject.hpp"
#include "EngineCore.h"
#include "CoreMinimal.h"

RTTR_PLUGIN_REGISTRATION
{
	ZoneScopedN("Registering RTTR")
	using namespace Core::Datastructure;
	registration::class_<ICamera>("ICamera")
		.method("GetCameraPerspective", &ICamera::GetPerspectiveMatrix)
		.method("GetCameraMatrix", &ICamera::GetCameraMatrix)
		.method("SetRatio", &ICamera::SetRatio);
}

const Core::Maths::Mat4& Core::Datastructure::ICamera::GetPerspectiveMatrix()
{
	ZoneScoped
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
	ZoneScoped
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
}

bool Core::Datastructure::ICamera::OnStart()
{
	ZoneScoped
	std::list<Renderer::Postprocess*> components;
	m_parent->GetComponentsOfTypeInObject<Renderer::Postprocess>(components);
	if (components.size() > 0)
		m_postProcessHandler = *components.begin();

	return IComponent::OnStart();
}

Core::Datastructure::ICamera::~ICamera() noexcept
{
	if (GetRoot() == nullptr)
		return;
	GetRoot()->RemoveCamera(this);
}

void Core::Datastructure::ICamera::DrawDepth(const std::list<IRenderable*>& renderables)
{
	std::vector<Core::Renderer::Light*> lights = Resources::Shader::GetShadowCastingLights();

	for (size_t i{ 0 }; i < lights.size(); ++i)
	{
		if (!lights[i]->GetShadowDrawn())
		{
			lights[i]->ResizeShadowTexture(m_fbo->Size[2], m_fbo->Size[3]);

			glBindFramebuffer(GL_FRAMEBUFFER, lights[i]->ShadowBuffer());
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glCullFace(GL_FRONT);
			for (auto it{ renderables.begin() }; it != renderables.end(); ++it)
				(*it)->Draw(lights[i]->GetViewFromLight(), this->GetPerspectiveMatrix(), m_shadowShader);
			glCullFace(GL_BACK);

			lights[i]->SetShadowDrawn(true);

			glActiveTexture(GL_TEXTURE2 + i);
			glBindTexture(GL_TEXTURE_2D, lights[i]->ShadowTexture());
		}
	}
}

void Core::Datastructure::ICamera::Draw(const std::list<Core::Datastructure::IRenderable*>& renderables)
{
	ZoneScoped
		ZoneText("Render of a camera", 22)
		TracyGpuZone("Rendering frame buffer")
	if (IsInit() && m_isActive && !IsDestroyed() && m_parent->IsActive())
	{
		glViewport(m_fbo->Size[0], m_fbo->Size[1], m_fbo->Size[2], m_fbo->Size[3]);
		
		DrawDepth(renderables);

		// Draw scene
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->FBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, attachments);

		for (auto it{ renderables.begin() }; it != renderables.end(); ++it)
			(*it)->Draw(this->GetCameraMatrix(), this->GetPerspectiveMatrix());

		if (m_postProcessHandler)
		{
			m_postProcessHandler->Resize(m_fbo->Size[2], m_fbo->Size[3]);
			m_postProcessHandler->DrawPostProcess(m_fbo);
		}
		
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
	}
}

void Core::Datastructure::ICamera::OnCopy(IComponent* copyTo) const
{
	ZoneScoped
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
	if (m_postProcessHandler)
		m_postProcessHandler->Resize(width, height);
	for (size_t i{ 0 }; i < Resources::Shader::lights.size(); ++i)
		Resources::Shader::lights[i]->ResizeShadowTexture(width, height);
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
