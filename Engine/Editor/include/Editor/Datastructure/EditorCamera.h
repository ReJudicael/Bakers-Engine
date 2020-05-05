#pragma once

#include "ComponentBase.h"
#include "ICamera.h"
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
	class EditorCamera : public virtual Core::Datastructure::ComponentBase, public virtual Core::Datastructure::ICamera
	{
		CameraPerspective	m_persp;
		bool				m_isPerspectiveUpdated = false;
		bool				m_isCamUpdated = false;
		Core::Datastructure::Transform		m_transform;

		Core::Maths::Vec3	m_angularMovement;
		Core::Maths::Vec3	m_movement;
		Core::Maths::Vec2	m_mousePos;
		float	m_runSpeed{ 5.f };
		float	m_yaw{ 0.f };
		float	m_pitch{ 0.f };
		bool	m_isRotating{ false };
		bool	m_isMouseSet{ false };

		//Should make speed change with mousewheel
		float				m_speed{ 5 };

		virtual void		OnInit() override;

		Core::Maths::Mat4	OnGenerateCamera() override;
		Core::Maths::Mat4	OnGeneratePerspective() override;
		virtual bool		IsCameraMatrixUpdated() override { return m_transform.IsTrsUpdated(); }
		virtual bool		IsPerspectiveMatrixUpdated() override { return m_isPerspectiveUpdated; }
		virtual void		PerspectiveMatrixUpdated() override { m_isPerspectiveUpdated = true; }


		virtual void StartCopy(IComponent*& copyTo) const override;
		virtual void OnCopy(IComponent* copyTo) const override;

		virtual void OnDestroy() override;
		virtual void OnReset() override;
	public:
		EditorCamera();
		void SetRatio(const float newRatio) override;

		Core::Renderer::Framebuffer* GetFBO();

		/**
		 * Get the camera forward rotated with given ratios
		 * @param ratioX: Proportion of fov angle used for rotation around up axis
		 * @param ratioY: Proportion of fov angle used for rotation around right axis
		 * @return: Direction from camera
		 */
		Core::Maths::Vec3 GetPerspectiveDirection(const float ratioX, const float ratioY);

		/**
		 * Set Rotation of camera transform
		 * @param v: Vector with euler angles for rotation
		 */
		void SetRot(const Core::Maths::Vec3& v);

		/**
		 * Set Position of camera transform
		 * @param v: Vector with new position
		 */
		void SetPos(const Core::Maths::Vec3& v);

		/**
		 * Get Camera Position
		 * @return Local position of Editor camera transform
		 */
		const Core::Maths::Vec3& GetPos();

		/**
		 * Update translation and rotation
		 * @param deltaTime: Time elapsed between two frames
		 */
		virtual void Update(float deltaTime);
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

		REGISTER_CLASS(Core::Datastructure::ComponentBase, Core::Datastructure::ICamera)
	};
}