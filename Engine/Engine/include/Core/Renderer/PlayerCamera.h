#pragma once

#include "Camera.h"
#include "Vec2.hpp"
#include "CoreMinimal.h"

namespace Core::Renderer
{
	/**
	 * Camera controllable through WASD and mouse inputs
	 */
	BAKERS_API_CLASS PlayerCamera : public virtual Camera
	{
	private:
		// Post process demonstration data
		std::vector<Core::Renderer::Framebuffer*> m_postProcessFBO;
		Core::Maths::Vec3 m_color{ 0.5f, 0.5f, 0.5 };
		bool m_colorActivated{ false };
		bool m_blurActivated{ false };
		bool m_bwActivated{ false };

		// Movement through WASD keys
		bool	m_isRunning{ false };
		float	m_speed{ 5 };
		float	m_runningSpeed{ 20 };
		float	m_minMoveLength{ 0.001f };
		float	m_moveLerpSpeed{ 0.5f };
		Core::Maths::Vec3 m_movement;

		// Rotation through mouse movements
		bool	m_isRotating{ false };
		bool	m_isMouseSet{ false };
		float	m_angularSpeed{ 0.1f };
		float	m_yaw{ 0.f };
		float	m_pitch{ 0.f };
		float	m_minAngularLength{ 0.001f };
		float	m_angularLerpSpeed{ 0.5f };
		Core::Maths::Vec3 m_angularMovement;
		Core::Maths::Vec2	m_mousePos;
	protected:
		/**
		 * Copies the data of the component into the given component.
		 * Should always be safe to cast pointer to current component type.
		 */
		virtual void	OnCopy(IComponent* copyTo) const override;

		/**
		 * Copies the component in the given pointer.
		 * On override, should not call other versions of the function.
		 */
		virtual void	StartCopy(IComponent*& copyTo) const override;
		virtual void	OnReset() override;

		/**
		 * Destroy event
		 */
		virtual void	OnDestroy() override;

		/**
		 * Create a vector with all postprocess framebuffers that are activated
		 * @return Vector with active postprocess framebuffers
		 */
		std::vector<Core::Renderer::Framebuffer*> GetActiveFramebuffers();

	public:
		/**
		 * Default Constructor
		 */
		PlayerCamera();

		/**
		 * Constructor initializing camera variables
		 */
		PlayerCamera(const float ratio, const float fov, const float near, const float far);

		/**
		 * First frame upon creation event
		 */
		virtual void				OnInit() override;

		/**
		 * Compute keyboard and mouse interactions when right mouse button is pressed
		 */
		void	MoveWithInput();

		/**
		 * Compute translation with WASD keys
		 */
		void	ComputeTranslation();

		/**
		 * Compute rotation with mouse movements
		 */
		void	ComputeRotation();

		/**
		 * Update translation and rotation
		 * @param deltaTime: Time elapsed between two frames
		 */
		virtual void OnUpdate(float deltaTime) override;

		/**
		 * Update translation with stored movement vector
		 * @param deltaTime: Time elapsed between two frames
		 */
		void	UpdatePosition(float deltaTime);

		/**
		 * Update rotation with stored rotation vector
		 * @param deltaTime: Time elapsed between two frames
		 */
		void	UpdateRotation(float deltaTime);

		/**
		 * Update movement vector (change applied by OnUpdate)
		 * @param move: New value for stored movement vector
		 */
		void	Move(Core::Maths::Vec3 move);

		/**
		 * Update rotation vector (change applied by OnUpdate)
		 * @param move: New value for stored angular movement vector
		 */
		void	Rotate(Core::Maths::Vec3 move);

		/**
		 * Call when viewport of the camera must be resized.
		 * It will resize the draw FBO and change its aspect ratio.
		 */
		virtual void				Resize(unsigned width, unsigned height);
		
		/**
		 * Function called by the engine to draw the objects
		 * @param renderables: Objects of the scene
		 */
		virtual void Draw(const std::list<Core::Datastructure::IRenderable*>& renderables);

		REGISTER_CLASS(Camera);
	};

}

