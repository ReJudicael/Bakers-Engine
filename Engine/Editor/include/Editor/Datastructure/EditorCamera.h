#pragma once

#include "ComponentBase.h"
#include "ICamera.h"
#include "Transform.hpp"

#include "Vec2.hpp"
#include "Vec3.hpp"

namespace Editor::Datastructure
{
	/**
	 * Camera perspective properties
	 */
	struct CameraPerspective
	{
		float width{ 1024.f };
		float height{ 780.f };
		float fov{ 60.f };
		float near{ 0.1f };
		float far{ 100.f };
	};

	/**
	 * Camera of the Editor (used in the WindowScene)
	 */
	class EditorCamera : public virtual Core::Datastructure::ComponentBase, public virtual Core::Datastructure::ICamera
	{
		bool m_isPerspectiveUpdated{ false };
		bool m_isCamUpdated{ false };
		bool m_isRotating{ false };
		bool m_isMouseSet{ false };

		float m_speed{ 5.f };
		float m_runSpeed{ 5.f };
		float m_yaw{ 0.f };
		float m_pitch{ 0.f };

		Core::Maths::Vec2 m_mousePos;
		Core::Maths::Vec3 m_angularMovement;
		Core::Maths::Vec3 m_movement;
		CameraPerspective m_persp;

		Core::Datastructure::Transform m_transform;

	public:
		/**
		 * Default constructor
		 */
		EditorCamera();

	private:
		virtual void OnInit() override;
		virtual bool OnStart() override;

		virtual void StartCopy(IComponent*& copyTo) const override;
		virtual void OnCopy(IComponent* copyTo) const override;

		virtual void OnDestroy() override;
		virtual void OnReset() override;

		virtual bool IsCameraMatrixUpdated() override;
		virtual bool IsPerspectiveMatrixUpdated() override;
		virtual void PerspectiveMatrixUpdated() override;

		Core::Maths::Mat4 OnGenerateCamera() override;
		Core::Maths::Mat4 OnGeneratePerspective() override;

	public:
		/**
		 * Update translation and rotation
		 * @param deltaTime: Time elapsed between two frames
		 */
		virtual void Update(float deltaTime, Core::SystemManagement::InputSystem* inupt);

		/**
		 * Set ratio with width and height
		 * @param width: Width of ratio
		 * @param height: Height of ratio
		 */
		void SetRatio(const float width, const float height) override;

		/**
		 * Get FrameBuffer
		 */
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

	private:
		/**
		 * Compute keyboard and mouse interactions when right mouse button is pressed
		 */
		void MoveWithInput(Core::SystemManagement::InputSystem* input);

		/**
		 * Check the editor selection mode and the left mouse button
		 * @ return true if the camera can be moved with current mouse movements
		 */
		bool IsUsingMouseTranslation(Core::SystemManagement::InputSystem* input);

		/**
		 * Compute translation with WASDQE keys
		 */
		void ComputeInputTranslation(Core::SystemManagement::InputSystem* input);

		/**
		 * Compute translation with mouse movements
		 */
		void ComputeMouseTranslation(Core::SystemManagement::InputSystem* input);

		/**
		 * Compute rotation with mouse movements
		 */
		void ComputeRotation(Core::SystemManagement::InputSystem* input);

		/**
		 * Update translation with stored movement vector
		 * @param deltaTime: Time elapsed between two frames
		 */
		void UpdatePosition(float deltaTime);

		/**
		 * Update rotation with stored rotation vector
		 * @param deltaTime: Time elapsed between two frames
		 */
		void UpdateRotation(float deltaTime);

		/**
		 * Update movement vector (change applied by OnUpdate)
		 * @param move: New value for stored movement vector
		 */
		void Move(Core::Maths::Vec3 move, Core::SystemManagement::InputSystem* input);

		/**
		 * Update rotation vector (change applied by OnUpdate)
		 * @param move: New value for stored angular movement vector
		 */
		void Rotate(Core::Maths::Vec3 move);

		REGISTER_CLASS(Core::Datastructure::ICamera)
	};
}