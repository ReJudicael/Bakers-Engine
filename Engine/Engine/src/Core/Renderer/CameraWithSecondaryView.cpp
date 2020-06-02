#include "CameraWithSecondaryView.h"

#include "RootObject.hpp"
#include "EngineCore.h"

RTTR_PLUGIN_REGISTRATION
{
	ZoneScopedN("Registering RTTR")
		ZoneText("Leak happening in this zone is from RTTR and is actually reflexion data. It is a purposeful leak", 98);
	rttr::registration::class_<Core::Renderer::CameraWithSecondaryView>("Camera with secondary view")
	.constructor()
	.property("Target fbo", &Core::Renderer::CameraWithSecondaryView::m_targetFbo)
	.property("fbo type", &Core::Renderer::CameraWithSecondaryView::m_typeFbo)
	.property("Secondary view pos", &Core::Renderer::CameraWithSecondaryView::GetViewPos, &Core::Renderer::CameraWithSecondaryView::SetViewPos)
	(
		rttr::metadata(MetaData_Type::SHOW_IN_EDITOR, "ShouldDisplayPosDataInInspector")
	)
	.property("Secondary view size", &Core::Renderer::CameraWithSecondaryView::GetViewSize, &Core::Renderer::CameraWithSecondaryView::SetViewSize)
	(
		rttr::metadata(MetaData_Type::SHOW_IN_EDITOR, "ShouldDisplayPosDataInInspector")
	)
	.property("Scroll speed", &Core::Renderer::CameraWithSecondaryView::m_speed)
	.property("Max zoom", &Core::Renderer::CameraWithSecondaryView::m_maxZoom)
	.property("Min zoom", &Core::Renderer::CameraWithSecondaryView::m_minZoom)
	.method("ShouldDisplayPosDataInInspector", &Core::Renderer::CameraWithSecondaryView::ShouldDisplayPosDataInInspector);
}

namespace Core::Renderer
{
	bool CameraWithSecondaryView::ShouldDisplayPosDataInInspector()
	{
		return !IsStarted();
	}

	CameraWithSecondaryView::CameraWithSecondaryView() : Camera()
	{
	}

	CameraWithSecondaryView::~CameraWithSecondaryView()
	{
	}

	void CameraWithSecondaryView::SetViewPos(Core::Maths::Vec2 viewPos)
	{
		m_viewPos = viewPos;
		if (IsInit())
			ResizeView();
	}

	void CameraWithSecondaryView::SetViewSize(Core::Maths::Vec2 viewSize)
	{
		m_viewSize = viewSize;
		if (IsInit())
			ResizeView();
	}

	void CameraWithSecondaryView::Draw(const std::list<Core::Datastructure::IRenderable*>& renderables)
	{
		Camera::Draw(renderables);

		glDepthFunc(GL_ALWAYS);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->FBO);

		Core::Renderer::Framebuffer* fbo{ GetRoot()->GetEngine()->GetFBO(m_targetFbo, m_typeFbo) };
		if (fbo == nullptr)
			return;

		GetRoot()->GetEngine()->GetResourcesManager()->GetShader("PostProcessDefault")->UseProgram();
		glBindVertexArray(m_VAO);
		glBindTexture(GL_TEXTURE_2D, fbo->ColorTexture);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDepthFunc(GL_LESS);

		if (fbo->Size[2] != static_cast<int>(m_viewSize.x * m_cameraWidth) || fbo->Size[3] != static_cast<int>(m_viewSize.y * m_cameraHeight))
		{
			//fbo->Resize(static_cast<float>(m_viewSize.x * m_cameraWidth), static_cast<float>(m_viewSize.y * m_cameraHeight));
			static_cast<ICamera*>(fbo->userPtr)->Resize(static_cast<unsigned>(m_viewSize.x * m_cameraWidth), static_cast<unsigned>(m_viewSize.y * m_cameraHeight));
		}
	}

	void CameraWithSecondaryView::ResizeView()
	{
		glBindVertexArray(m_VAO);
		{
			float botLeftx = -1.f + m_viewPos.x * 2.f;
			float botLefty = -1.f + m_viewPos.y * 2.f;
			float botRightx = -1.f + m_viewPos.x * 2.f + m_viewSize.x * 2.f;
			float botRighty = -1.f + m_viewPos.y * 2.f;
			float topLeftx = -1.f + m_viewPos.x * 2.f;
			float topLefty = -1.f + m_viewPos.y * 2.f + m_viewSize.y * 2.f;
			float topRightx = -1.f + m_viewPos.x * 2.f + m_viewSize.x * 2.f;
			float topRighty = -1.f + m_viewPos.y * 2.f + m_viewSize.y * 2.f;
			float Quad[24] =
			{
				 botLeftx, botLefty,  0.f, 0.f, // bl
				 botRightx, botRighty,  1.f, 0.f, // br
				 topRightx, topRighty, 1.f, 1.f, // tr

				 topLeftx, topLefty, 0.f, 1.f , // tl
				 botLeftx, botLefty, 0.f, 0.f, // bl
				 topRightx, topRighty, 1.f, 1.f, // tr
			};

			// Upload mesh to gpu
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), Quad, GL_STATIC_DRAW);
		}
	}

	void CameraWithSecondaryView::OnInit()
	{
		Camera::OnInit();
		ZoneScoped

			glGenBuffers(1, &m_VBO);

		ResizeView();

		// Create a vertex array and bind it
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glBindVertexArray(0);

		GetRoot()->GetEngine()->GetResourcesManager()->CreateShader("PostProcessDefault", "Resources\\Shaders\\PostprocessShader.vert", "Resources\\Shaders\\SkyboxShader.frag");
	}

	void CameraWithSecondaryView::OnCopy(IComponent* copyTo) const
	{
		ZoneScoped
			Camera::OnCopy(copyTo);

		CameraWithSecondaryView* copy = dynamic_cast<CameraWithSecondaryView*>(copyTo);

		copy->m_targetFbo = m_targetFbo;
		copy->m_typeFbo = m_typeFbo;
	}

	void CameraWithSecondaryView::StartCopy(IComponent*& copyTo) const
	{
		copyTo = new CameraWithSecondaryView();
		OnCopy(copyTo);
	}

	void CameraWithSecondaryView::OnUpdate(float deltaTime)
	{
		if (Input()->IsScrollUp())
		{
			float newMovement = m_speed * deltaTime;
			float move = (m_currentZoom + newMovement <= m_maxZoom) ? newMovement : m_maxZoom - m_currentZoom;
			m_parent->Translate(Maths::Vec3{ 0, 0, 1 } * move);
			m_currentZoom += move;
		}

		if (Input()->IsScrollDown())
		{
			float newMovement = -1 * m_speed * deltaTime;
			float move = (m_currentZoom + newMovement >= m_minZoom) ? newMovement : m_minZoom - m_currentZoom;
			m_parent->Translate(Maths::Vec3{ 0, 0, 1 } * move);
			m_currentZoom += move;
		}
	}
}
