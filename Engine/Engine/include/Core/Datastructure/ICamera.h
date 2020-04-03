#pragma once

#include "Vec3.hpp"
#include "Mat4.hpp"
#include "IComponent.hpp"
#include "Framebuffer.h"
#include "CoreMinimal.h"

namespace Core::Datastructure
{
	class IRenderable;
	BAKERS_API_CLASS ICamera : public virtual IComponent
	{
	private:
		Core::Maths::Mat4	m_perspective;
		Core::Maths::Mat4	m_cameraMatrix;
	protected:
		Core::Maths::Vec3	m_front;
		Core::Maths::Vec3	m_right;

		unsigned			m_cameraWidth{1280};
		unsigned			m_cameraHeight{800};

		Core::Renderer::Framebuffer* m_fbo;

		bool				m_isPerspectiveUpdated{ false };
		bool				m_isCamUpdated{ false };

		virtual Core::Maths::Mat4	OnGeneratePerspective() = 0;
		virtual Core::Maths::Mat4	OnGenerateCamera() = 0;

		virtual void	OnCopy(IComponent * copyTo) const override;
	public:
		/**
		 * Destructor of the camera interface
		 */
		~ICamera() noexcept;

		/**
		 * Get perspective matrix of the camera
		 * @return Stored perspective matrix
		 */
		const Core::Maths::Mat4&	GetPerspectiveMatrix();
		/**
		 * Get reverse TRS matrix of the camera
		 * @return Reverse TRS camera matrix
		 */
		const Core::Maths::Mat4&	GetCameraMatrix();

		virtual void				OnStart() override;

		virtual void				SetRatio(float ratio) = 0;

		Core::Maths::Vec3			GetFront() const noexcept
		{
			return m_front;
		}
		Core::Maths::Vec3			GetRight() const noexcept
		{
			return m_right;
		}

		/**
		 * Function called by the engine to draw the objects
		 * @param renderables: Objects of the scene
		 */
		void Draw(const std::list<IRenderable*>& renderables);
		
		REGISTER_CLASS(IComponent)
	};
}

