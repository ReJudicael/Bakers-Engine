#pragma once

#include "ComponentBase.h"
#include "ICamera.h"
#include "IUpdatable.hpp"
#include "CoreMinimal.h"

namespace Core::Renderer
{
	/**
	 * Store data needed to create camera perspective matrix
	 */
	struct CameraPerspective
	{
		float width = 1024.0f;
		float height = 780.0f;
		float fov = 60.0f;
		float near = 0.1f;
		float far = 100.f;

		REGISTER_CLASS()
	};

	/**
	 * Camera Component, create camera and perspective matrices
	 */
	BAKERS_API_CLASS Camera : public virtual Core::Datastructure::ComponentBase, public virtual Core::Datastructure::ICamera, public virtual Core::Datastructure::IUpdatable
	{
	protected:
		CameraPerspective m_persp;

		bool				m_isPerspectiveUpdated{ false };

		/**
		 * Generate and return camera matrix
		 * @return Reversed transformation matrix of the camera game object
		 */
		Core::Maths::Mat4	OnGenerateCamera() override;
		virtual bool		IsCameraMatrixUpdated() override;
		virtual bool		IsPerspectiveMatrixUpdated() override { return m_isPerspectiveUpdated; }
		virtual void		PerspectiveMatrixUpdated() override { m_isPerspectiveUpdated = true; }

		/**
		 * Generate and return perspective matrix
		 * @return Perspective matrix created with current perspective data
		 */
		Core::Maths::Mat4	OnGeneratePerspective() override;

		/**
		 * Copies the data of the component into the given component.
		 * Should always be safe to cast pointer to current component type.
		 */
		virtual void OnCopy(IComponent* copyTo) const override;

		/**
		 * Copies the component in the given pointer.
		 * On override, should not call other versions of the function.
		 */
		virtual void StartCopy(IComponent*& copyTo) const override;

		/**
		 * Destroy event
		 */
		virtual void OnDestroy() override;
		virtual void OnReset() override;
	public:
		/**
		 * Default Constructor
		 */
		Camera();

		/**
		 * Constructor initializing camera variables
		 * @param width: Width of the window
		 * @param height: Height of the window
		 * @param fov: Angle of the field of view of the camera
		 * @param near: Distance where objects start to be rendered
		 * @param far: Distance where objects are no longer rendered
		 */
		Camera(const float width, const float height, const float fov, const float near, const float far);

		/**
		 * Update event
		 * @param deltaTime: Time elapsed between two frames
		 */
		virtual void OnUpdate(float deltaTime) override;

		/**
		 * First frame upon creation event
		 */
		virtual bool OnStart() override;

		/**
		 * Set window ratio to camera perspective data
		 * @param width: New window width for camera ratio
		 * @param height: New window height for camera ratio
		 */
		void SetRatio(const float width, const float height) override;

		/**
		 * Perspective data (ratio, fov, near, far) getter
		 * @return Current Perspective data of camera
		 */
		inline CameraPerspective GetPerspData() const { return m_persp; }

		/**
		 * Perspective data setter
		 * @param p: New set of Perspective data for the camera
		 */
		inline void SetPerspData(const CameraPerspective p)
		{
			m_persp = p;
			m_isPerspectiveUpdated = false;
		}

		/**
		 * Get the camera forward rotated with given ratios
		 * @param ratioX: Proportion of fov angle used for rotation around up axis
		 * @param ratioY: Proportion of fov angle used for rotation around right axis
		 * @return: Direction from camera
		 */
		Core::Maths::Vec3 GetPerspectiveDirection(const float ratioX, const float ratioY);

		/**
		 * Create Perspective projection Matrix
		 * @param width: Width of the window
		 * @param height: Height of the window
		 * @param fov: Angle of the field of view of the camera
		 * @param near: Distance where objects start to be rendered
		 * @param far: Distance where objects are no longer rendered
		 * @return Perspective matrix
		 */
		static Core::Maths::Mat4 CreatePerspectiveMatrix(const float width, const float height, const float fov, const float near, const float far);

		/**
		 * Create Orthographic projection Matrix
		 * @param width: Width of the window
		 * @param height: Height of the window
		 * @param near: Distance where objects start to be rendered
		 * @param far: Distance where objects are no longer rendered
		 * @return Perspective matrix
		 */
		static Core::Maths::Mat4 CreateOrthographicMatrix(const float width, const float height, const float near, const float far);

		REGISTER_CLASS(Core::Datastructure::ComponentBase, Core::Datastructure::ICamera, Core::Datastructure::IUpdatable)
	};
}
