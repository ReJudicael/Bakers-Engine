#pragma once

#include "CoreMinimal.h"
#include "Camera.h"
#include "ComponentBase.h"
#include "Framebuffer.h"
#include "Vec2.hpp"

namespace Core::Renderer
{
	BAKERS_API_CLASS CameraWithSecondaryView : public Camera
	{
	private:
		int						m_targetFbo { 1 };
		FBOType m_typeFbo{ FBOType::CAMERA };
		Core::Maths::Vec2		m_viewPos{ 0.75f, 0.75f };
		Core::Maths::Vec2		m_viewSize{ 0.25f, 0.25f };
		GLuint					m_VAO;
		GLuint					m_VBO;

		float					m_speed{ 2 };
		float					m_currentZoom{ 0 };
		float					m_maxZoom{ 3 };
		float					m_minZoom{ -3 };

		bool					ShouldDisplayPosDataInInspector();

		void					ResizeView();
	public:
		CameraWithSecondaryView();
		~CameraWithSecondaryView();

		void				SetViewPos(Core::Maths::Vec2 viewPos);
		Core::Maths::Vec2	GetViewPos() { return m_viewPos; }
		void				SetViewSize(Core::Maths::Vec2 viewSize);
		Core::Maths::Vec2	GetViewSize() { return m_viewSize; }

		virtual void	Draw(const std::list<Core::Datastructure::IRenderable*>& renderables) override;
		virtual void	OnInit() override;

		virtual void	OnCopy(IComponent* copyTo) const override;
		virtual void	StartCopy(IComponent*& copyTo) const override;

		/**
		 * Update event
		 * @param deltaTime: Time elapsed between two frames
		 */
		virtual void OnUpdate(float deltaTime) override;

		REGISTER_CLASS(Camera);
	};
}
