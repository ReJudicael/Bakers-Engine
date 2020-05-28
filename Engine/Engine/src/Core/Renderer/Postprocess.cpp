#include "Postprocess.h"
#include "EngineCore.h"
#include "RootObject.hpp"

RTTR_PLUGIN_REGISTRATION
{
	registration::class_<Core::Renderer::Postprocess>("Postprocess")
		.constructor();
}

namespace Core::Renderer
{
	Postprocess::Postprocess() : ComponentBase()
	{}

	Postprocess::~Postprocess()
	{
	}

	void	Postprocess::OnCopy(IComponent* copyTo) const
	{
		ZoneScoped
			ComponentBase::OnCopy(copyTo);

		Postprocess* copy{ dynamic_cast<Postprocess*>(copyTo) };
	}

	void	Postprocess::StartCopy(IComponent*& copyTo) const
	{
		ZoneScoped
			copyTo = new Postprocess();
		OnCopy(copyTo);
	}

	bool	Postprocess::OnStart()
	{
		ZoneScoped
			return ComponentBase::OnStart();
	}

	void    Postprocess::OnDestroy()
	{
		ComponentBase::OnDestroy();
		GetRoot()->GetEngine()->DeleteFBO(m_effectFbo);
		GetRoot()->GetEngine()->DeleteFBO(m_addedFbo);
	}

	void    Postprocess::OnReset()
	{
		ComponentBase::OnReset();

		if (m_effectFbo != nullptr)
		{
			GetRoot()->GetEngine()->DeleteFBO(m_effectFbo);
			m_effectFbo = nullptr;
		}
		if (m_addedFbo != nullptr)
		{
			GetRoot()->GetEngine()->DeleteFBO(m_addedFbo);
			m_addedFbo = nullptr;
		}
	}

	void	Postprocess::OnInit()
	{
		ComponentBase::OnInit();

		m_effectFbo = GetRoot()->GetEngine()->CreateFBO(571, 391, Renderer::FBOType::POSTPROCESSING);
		m_effectFbo->userPtr = this;
		m_effectFbo->InitPostProcess(GetRoot()->GetEngine()->GetResourcesManager()->CreateShader("PostProcess", "Resources\\Shaders\\PostprocessShader.vert", "Resources\\Shaders\\PostprocessShader.frag"));

		m_addedFbo = GetRoot()->GetEngine()->CreateFBO(571, 391, Renderer::FBOType::POSTPROCESSING);
		m_addedFbo->userPtr = this;

		m_colorCustomShader = GetRoot()->GetEngine()->GetResourcesManager()->CreateShader("PostProcess", "Resources\\Shaders\\PostprocessShader.vert", "Resources\\Shaders\\PostprocessShader.frag");
		m_blurShader = GetRoot()->GetEngine()->GetResourcesManager()->CreateShader("PostProcessBlur", "Resources\\Shaders\\PostprocessBlurShader.vert", "Resources\\Shaders\\PostprocessBlurShader.frag");
		m_colorBWShader = GetRoot()->GetEngine()->GetResourcesManager()->CreateShader("PostProcessBW", "Resources\\Shaders\\PostprocessShader.vert", "Resources\\Shaders\\PostprocessBlackWhiteShader.frag");
	}

	void	Postprocess::DrawPostProcess(Framebuffer* drawFbo)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_effectFbo->FBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_blurShader->UseProgram();
		glBindTexture(GL_TEXTURE_2D, drawFbo->ColorTexture);
		glBindVertexArray(m_effectFbo->data.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindFramebuffer(GL_FRAMEBUFFER, drawFbo->FBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_effectFbo->data.s->UseProgram();
		glUniform3fv(m_effectFbo->data.s->GetLocation("uColor"), 1, Maths::Vec3(1, 1, 1).xyz);
		glBindTexture(GL_TEXTURE_2D, m_effectFbo->ColorTexture);
		glBindVertexArray(m_effectFbo->data.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

	}

	void	Postprocess::Resize(unsigned width, unsigned height)
	{
		m_effectFbo->Resize(width, height);
		m_addedFbo->Resize(width, height);
	}
}