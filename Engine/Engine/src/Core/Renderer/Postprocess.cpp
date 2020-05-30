#include "Postprocess.h"
#include "EngineCore.h"
#include "RootObject.hpp"

RTTR_PLUGIN_REGISTRATION
{
	registration::class_<Core::Renderer::Postprocess>("Postprocess")
		.constructor()
		.property("Bloom", &Core::Renderer::Postprocess::m_useBloom)
		.property("Blur", &Core::Renderer::Postprocess::m_useBlur)
		(
			rttr::metadata(MetaData_Type::SHOW_IN_EDITOR, "CanUseEffect")
		)
		.property("Custom Color", &Core::Renderer::Postprocess::m_useColorCustom)
		(
			rttr::metadata(MetaData_Type::SHOW_IN_EDITOR, "CanUseEffect")
		)
		.property("Color", &Core::Renderer::Postprocess::m_correctedColor)
		(
			rttr::metadata(MetaData_Type::SHOW_IN_EDITOR, "IsUsingColor")
		)
		.property("Black & White", &Core::Renderer::Postprocess::m_useBW)
		(
			rttr::metadata(MetaData_Type::SHOW_IN_EDITOR, "CanUseEffect")
		)
		.property("Black & White", &Core::Renderer::Postprocess::m_useBW)
		(
			rttr::metadata(MetaData_Type::SHOW_IN_EDITOR, "CanUseEffect")
		)
		.property("Cartoon effect", &Core::Renderer::Postprocess::m_useCartoon)
		(
			rttr::metadata(MetaData_Type::SHOW_IN_EDITOR, "CanUseEffect")
		)
		.property("Line intensity", &Core::Renderer::Postprocess::m_linesThreshold)
		(
			rttr::metadata(MetaData_Type::SHOW_IN_EDITOR, "IsUsingCartoon")
		)
		.property("Line color", &Core::Renderer::Postprocess::m_linesColor)
		(
			rttr::metadata(MetaData_Type::SHOW_IN_EDITOR, "IsUsingCartoon")
		)
		.method("IsUsingBlur", &Core::Renderer::Postprocess::IsUsingBlur)
		.method("IsUsingColor", &Core::Renderer::Postprocess::IsUsingColor)
		.method("IsUsingCartoon", &Core::Renderer::Postprocess::IsUsingCartoon)
		.method("CanUseEffect", &Core::Renderer::Postprocess::CanUseEffects);
}

namespace Core::Renderer
{
	Postprocess::Postprocess() : ComponentBase()
	{}

	Postprocess::~Postprocess()
	{}

	void	Postprocess::OnCopy(IComponent* copyTo) const
	{
		ZoneScoped
			ComponentBase::OnCopy(copyTo);

		Postprocess* copy{ dynamic_cast<Postprocess*>(copyTo) };

		copy->m_useBlur = m_useBlur;
		copy->m_useColorCustom = m_useColorCustom;
		copy->m_useBW = m_useBW;
		copy->m_useBloom = m_useBloom;

		copy->m_VAO = m_VAO;

		copy->m_correctedColor = m_correctedColor;

		copy->m_effectFbo = m_effectFbo;
		copy->m_addedFbo = m_addedFbo;

		copy->m_defaultShader = m_defaultShader;
		copy->m_blurShader = m_blurShader;
		copy->m_colorCustomShader = m_colorCustomShader;
		copy->m_colorBWShader = m_colorBWShader;
		copy->m_additionShader = m_additionShader;
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
		glDeleteVertexArrays(1, &m_VAO);
	}

	void    Postprocess::OnReset()
	{
		ComponentBase::OnReset();

		m_useBlur = false;
		m_useColorCustom = false;
		m_useBW = false;
		m_useBloom = false;

		m_correctedColor = { 1, 1, 1, 1 };
	}

	void	Postprocess::OnInit()
	{
		ComponentBase::OnInit();

		InitPostprocessVAO();

		m_effectFbo = GetRoot()->GetEngine()->CreateFBO(571, 391, Renderer::FBOType::POSTPROCESSING);
		m_effectFbo->userPtr = this;

		m_addedFbo = GetRoot()->GetEngine()->CreateFBO(571, 391, Renderer::FBOType::POSTPROCESSING);
		m_addedFbo->userPtr = this;

		m_defaultShader = GetRoot()->GetEngine()->GetResourcesManager()->CreateShader("PostProcessDefault", "Resources\\Shaders\\PostprocessShader.vert", "Resources\\Shaders\\SkyboxShader.frag");
		m_colorCustomShader = GetRoot()->GetEngine()->GetResourcesManager()->CreateShader("PostProcess", "Resources\\Shaders\\PostprocessShader.vert", "Resources\\Shaders\\PostprocessShader.frag");
		m_blurShader = GetRoot()->GetEngine()->GetResourcesManager()->CreateShader("PostProcessBlur", "Resources\\Shaders\\PostprocessBlurShader.vert", "Resources\\Shaders\\PostprocessBlurShader.frag");
		m_colorBWShader = GetRoot()->GetEngine()->GetResourcesManager()->CreateShader("PostProcessBW", "Resources\\Shaders\\PostprocessShader.vert", "Resources\\Shaders\\PostprocessBlackWhiteShader.frag");
		m_additionShader = GetRoot()->GetEngine()->GetResourcesManager()->CreateShader("PostProcessAddition", "Resources\\Shaders\\PostprocessShader.vert", "Resources\\Shaders\\PostprocessAdditionShader.frag");
		m_cartoonShader = GetRoot()->GetEngine()->GetResourcesManager()->CreateShader("PostProcessToyEffect", "Resources\\Shaders\\PostprocessShader.vert", "Resources\\Shaders\\PostprocessCartoonShader.frag");
	}

	void	Postprocess::InitPostprocessVAO()
	{
		ZoneScoped
			GLuint VBO;

		// Gen unit quad
		{
			float Quad[24] =
			{
				 -1.f,-1.f ,  0.f, 0.f, // bl
				  1.f,-1.f ,  1.f, 0.f, // br
				  1.f, 1.f,  1.f, 1.f, // tr

				-1.f, 1.f ,  0.f, 1.f , // tl
				-1.f,-1.f,   0.f, 0.f, // bl
				 1.f, 1.f ,  1.f, 1.f, // tr
			};

			// Upload mesh to gpu
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), Quad, GL_STATIC_DRAW);
		}

		// Create a vertex array and bind it
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glBindVertexArray(0);
	}

	void	Postprocess::DrawEffect(Framebuffer* from, Framebuffer* to, std::shared_ptr<Resources::Shader> effect)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, to->FBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		effect->UseProgram();
		if (effect == m_colorCustomShader)
			glUniform3fv(effect->GetLocation("uColor"), 1, m_correctedColor.rgb);
		else if (effect == m_cartoonShader)
		{
			Maths::Vec2 resolution{ (float)m_effectFbo->Size[2],(float)m_effectFbo->Size[3] };
			glUniform2fv(effect->GetLocation("uResolution"), 1, resolution.xy);
			glUniform1f(effect->GetLocation("uThresold"), 1.f - m_linesThreshold);
			glUniform3fv(effect->GetLocation("uLineColor"), 1, m_linesColor.rgb);
		}
		glBindTexture(GL_TEXTURE_2D, from->ColorTexture);
		glBindVertexArray(m_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void	Postprocess::DrawBloom(Framebuffer* drawFbo)
	{
		// Store default image into second framebuffer
		DrawEffect(drawFbo, m_addedFbo, m_defaultShader);

		// Store bloom on bright objects into effect framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, m_effectFbo->FBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_blurShader->UseProgram();
		glBindTexture(GL_TEXTURE_2D, drawFbo->EffectTexture);
		glBindVertexArray(m_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Add scene and bright parts to create final display
		glBindFramebuffer(GL_FRAMEBUFFER, drawFbo->FBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_additionShader->UseProgram();
		// Init value of texture1 for mesh texture
		glUniform1i(m_additionShader->GetLocation("uColorTexture"), 0);
		// Init value of texture2 for normal map
		glUniform1i(m_additionShader->GetLocation("uSecondTexture"), 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_addedFbo->ColorTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_effectFbo->ColorTexture);
		glBindVertexArray(m_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void	Postprocess::DrawPostProcess(Framebuffer* drawFbo)
	{
		if (m_useBloom)
		{
			DrawBloom(drawFbo);
			return;
		}

		if (m_useBlur)
		{
			DrawEffect(drawFbo, m_effectFbo, m_blurShader);
			DrawEffect(m_effectFbo, drawFbo, m_defaultShader);
		}

		if (m_useColorCustom)
		{
			DrawEffect(drawFbo, m_effectFbo, m_colorCustomShader);
			DrawEffect(m_effectFbo, drawFbo, m_defaultShader);
		}

		if (m_useBW)
		{
			DrawEffect(drawFbo, m_effectFbo, m_colorBWShader);
			DrawEffect(m_effectFbo, drawFbo, m_defaultShader);
		}

		if (m_useCartoon)
		{
			DrawEffect(drawFbo, m_effectFbo, m_cartoonShader);
			DrawEffect(m_effectFbo, drawFbo, m_defaultShader);
		}
	}

	void	Postprocess::Resize(unsigned width, unsigned height)
	{
		m_effectFbo->Resize(width, height);
		m_addedFbo->Resize(width, height);
	}
}