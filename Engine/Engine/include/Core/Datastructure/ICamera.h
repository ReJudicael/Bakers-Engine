#pragma once

#include "Vec3.hpp"
#include "Mat4.hpp"
#include "IComponent.hpp"
#include "Framebuffer.h"
#include "CoreMinimal.h"

namespace Core::Datastructure
{
	class IRenderable;

	/**
	 * Interface class for Camera Component
	 */
	BAKERS_API_CLASS ICamera : public virtual IComponent
	{
	private:
		Core::Maths::Mat4	m_perspective;
		Core::Maths::Mat4	m_cameraMatrix;
	protected:
		unsigned			m_cameraWidth{1280};
		unsigned			m_cameraHeight{800};

		Core::Renderer::Framebuffer* m_fbo{ nullptr };
		std::shared_ptr<Resources::Shader> m_shadowShader{ nullptr };

		/**
		 * Generate and return camera matrix
		 * @return Reversed transformation matrix of the camera game object
		 */
		virtual Core::Maths::Mat4	OnGeneratePerspective() = 0;

		/**
		 * Generate and return perspective matrix
		 * @return Perspective matrix created with current perspective data
		 */
		virtual Core::Maths::Mat4	OnGenerateCamera() = 0;

		/**
		 * Copies the data of the component into the given component.
		 * Should always be safe to cast pointer to current component type.
		 */
		virtual void	OnCopy(IComponent * copyTo) const override;

		/**
		 * Destroy event
		 */
		virtual void	OnDestroy() override;

		virtual void	OnReset() override;

		/**
		 * Called after updating perspective matrix
		 */
		virtual void	PerspectiveMatrixUpdated() {}
		/**
		 * Called after updating camera matrix
		 */
		virtual void	CameraMatrixUpdated() {}
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
		 * Returns if perspective matrix is updated
		 */
		virtual bool				IsPerspectiveMatrixUpdated() = 0;
		/**
		 * Get reverse TRS matrix of the camera
		 * @return Reverse TRS camera matrix
		 */
		const Core::Maths::Mat4&	GetCameraMatrix();
		/**
		 * Returns if camera matrix is updated
		 */
		virtual bool				IsCameraMatrixUpdated() = 0;

		/**
		 * First frame upon creation event
		 */
		virtual bool				OnStart() override;
		virtual void				OnInit() override;

		/**
		 * Set window ratio to camera perspective data
		 * @param newRatio: New ratio to be applied
		 */
		virtual void				SetRatio(const float width, const float height) = 0;

		/**
		 * Call when viewport of the camera must be resized. 
		 * It will resize the draw FBO and change its aspect ratio.
		 */
		virtual void				Resize(unsigned width, unsigned height);

		/**
		 * Function called by the engine to draw the objects
		 * @param renderables: Objects of the scene
		 */
		virtual void Draw(const std::list<IRenderable*>& renderables);

		/**
		 * Draw objects into depth buffer for shadow mapping
		 * @param renderables: Objects of the scene
		 */
		void DrawDepth(const std::list<IRenderable*>& renderables);
		
		REGISTER_CLASS(IComponent)
	};
}

