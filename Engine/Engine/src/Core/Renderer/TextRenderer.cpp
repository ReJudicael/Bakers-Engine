#include "TextRenderer.h"
#include "RootObject.hpp"
#include "EngineCore.h"

using namespace Core::Maths;

RTTR_PLUGIN_REGISTRATION
{
	ZoneScopedN("Registering RTTR")
		ZoneText("Leak happening in this zone is from RTTR and is actually reflexion data. It is a purposeful leak", 98);
	using namespace Core::Renderer;
	rttr::registration::class_<TextRenderer>("Text Renderer")
		.constructor()
		.property("Text", &TextRenderer::m_text)
		.property("Font", &TextRenderer::m_font)
		.property("Size", &TextRenderer::m_size);
}

namespace Core::Renderer
{
	TextRenderer::TextRenderer()
	{
	}
	void TextRenderer::OnInit()
	{
		IRenderable::OnInit();
		ComponentBase::OnInit();

		m_quad = GetRoot()->GetEngine()->GetResourcesManager()->GetModel("Quad");
		m_shader = GetRoot()->GetEngine()->GetResourcesManager()->GetShader("Text");
	}

	void TextRenderer::OnDraw(const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj, std::shared_ptr<Resources::Shader> givenShader)
	{
		ZoneScoped
			TracyGpuZone("Render text")
		if (givenShader)
			return;
		if (m_text.size() == 0 || m_font.size() == 0 || GetRoot()->GetEngine()->GetResourcesManager()->GetFont(m_font) == nullptr || m_quad->stateVAO != Resources::EOpenGLLinkState::ISLINK)
			return;

		float len = 0;
		std::map<GLchar, Resources::Character*>::iterator  it;
		Resources::Character* c = nullptr;
		for (unsigned i{ 0 }; i < m_text.size(); ++i)
		{
			c = GetRoot()->GetEngine()->GetResourcesManager()->GetCharacter(m_font, m_text[i]);
			len += (c->advance >> 6);
		}
		len *= m_size / 9000.f;

		m_shader->UseProgram(); 
		glBindVertexArray(m_quad->VAOModel);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUniform3fv(m_shader->GetLocation("umat.ambientColor"), 1, Vec3(1, 1, 1).xyz);
		glUniform3fv(m_shader->GetLocation("umat.diffuseColor"), 1, Vec3(1, 1, 1).xyz);
		glUniform3fv(m_shader->GetLocation("umat.specularColor"), 1, Vec3(1, 1, 1).xyz);
		glUniform1f(m_shader->GetLocation("umat.shininess"), 1.f);
		glUniform1f(m_shader->GetLocation("umat.shininessStrength"), 1.f);
		for (int j{ 0 }; j < 10; ++j)
			glUniform1i(m_shader->GetLocation("uShadowMap[" + std::to_string(j) + "]"), 2 + j);
		
		std::vector<Core::Renderer::Light*> lights = Resources::Shader::GetShadowCastingLights();
		for (auto j{ 0 }; j < lights.size(); ++j)
			glUniformMatrix4fv(m_shader->GetLocation("uLightPOV[" + std::to_string(j) + "].view"), 1, GL_TRUE, lights[j]->GetViewFromLight().array);
		glUniform1i(m_shader->GetLocation("uShadowCount"), static_cast<GLint>(lights.size()));

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(m_shader->GetLocation("uColorTexture"), 0);
		glUniformMatrix4fv(m_shader->GetLocation("uCam"), 1, GL_TRUE, view.array);
		glUniformMatrix4fv(m_shader->GetLocation("uProj"), 1, GL_FALSE, proj.array);

		Core::Datastructure::Transform pos;
		pos.Translate(GetParent()->Forward() * len);
		Vec3 scale{ pos.GetLocalScale() * Core::Maths::Vec3(m_size / 100.f) };

		for (unsigned i{ 0 }; i < m_text.size(); ++i)
		{
			ZoneScopedN("Render single char")
				TracyGpuZone("Render single character")
			c = GetRoot()->GetEngine()->GetResourcesManager()->GetCharacter(m_font, m_text[i]);
			glBindTexture(GL_TEXTURE_2D, c->textureID);

			pos.Translate((GetParent()->Forward() * -(float)(c->advance >> 6) * static_cast<float>(m_size)) / 9000.f);
			pos.Translate((GetParent()->Up() * (float)(c->sizeY - c->bearingY) * static_cast<float>(m_size)) / 9000.f);
			pos.SetLocalScale(scale);
			pos.Scale({ c->sizeX / 48.f, c->sizeY / 48.f, 1 });

			pos.UpdatePos(GetParent()->GetUpdatedTransform());

			glUniformMatrix4fv(m_shader->GetLocation("uModel"), 1, GL_TRUE, pos.GetGlobalTrs().array);
			pos.Translate((GetParent()->Forward() * -(float)(c->advance >> 6) * static_cast<float>(m_size)) / 9000.f);
			pos.Translate((GetParent()->Up() * -(float)(c->sizeY - c->bearingY) * static_cast<float>(m_size)) / 9000.f);

			glDrawElements(GL_TRIANGLES, m_quad->offsetsMesh[0].count, GL_UNSIGNED_INT,
				(GLvoid*)(m_quad->offsetsMesh[0].beginIndices * sizeof(GLuint)));
		}

		glBindVertexArray(0);
	}

	void TextRenderer::OnReset()
	{
		IRenderable::OnReset();
		ComponentBase::OnReset();

		m_text = "Add text here";
	}

	void TextRenderer::OnCopy(IComponent* copyTo) const
	{
		ComponentBase::OnCopy(copyTo);
		IRenderable::OnCopy(copyTo);

		TextRenderer* text = dynamic_cast<TextRenderer*>(copyTo);

		text->m_text = m_text;
	}

	void TextRenderer::StartCopy(IComponent*& copyTo) const
	{
		ZoneScoped
		copyTo = new TextRenderer();
		OnCopy(copyTo);
	}
}
