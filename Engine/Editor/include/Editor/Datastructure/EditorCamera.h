#pragma once

#include "ComponentBase.h"
#include "ICamera.h"
#include "IUpdatable.hpp"
#include "Transform.hpp"

#include "Vec2.hpp"
#include "Vec3.hpp"

namespace Editor::Datastructure
{
	struct CameraPerspective
	{
		float ratio = 1.714f;
		float fov = 60.0f;
		float near = 0.1f;
		float far = 100.f;
	};
	class EditorCamera : public virtual Core::Datastructure::ComponentBase, public virtual Core::Datastructure::ICamera, public virtual Core::Datastructure::IUpdatable
	{
		CameraPerspective	m_persp;
		Core::Datastructure::Transform		m_transform;

		Core::Maths::Vec3	m_angularMovement;
		Core::Maths::Vec3	m_movement;
		Core::Maths::Vec2	m_mousePos;
		float	m_yaw{ 0.f };
		float	m_pitch{ 0.f };
		bool	m_isRotating{ false };
		bool	m_isMouseSet{ false };

		//Should make speed change with mousewheel
		float				m_speed{ 5 };

		virtual void		OnStart() override;

		Core::Maths::Mat4	OnGenerateCamera() override;
		Core::Maths::Mat4	OnGeneratePerspective() override;


		virtual void StartCopy(IComponent*& copyTo) const override;
		virtual void OnCopy(IComponent* copyTo) const override;

		virtual void OnDestroy() override;
		virtual void OnReset() override;
	public:
		EditorCamera();
		void SetRatio(const float newRatio) override;

		Core::Renderer::Framebuffer* GetFBO();
	private:
		/**
		 * Compute keyboard and mouse interactions when right mouse button is pressed
		 */
		void	MoveWithInput();

		/**
		 * Check the editor selection mode and the left mouse button
		 * @ return true if the camera can be moved with current mouse movements
		 */
		bool	IsUsingMouseTranslation();

		/**
		 * Compute translation with WASDQE keys
		 */
		void	ComputeInputTranslation();

		/**
		 * Compute translation with mouse movements
		 */
		void	ComputeMouseTranslation();

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

		REGISTER_CLASS(Core::Datastructure::ComponentBase, Core::Datastructure::ICamera, Core::Datastructure::IUpdatable)
	};
}

