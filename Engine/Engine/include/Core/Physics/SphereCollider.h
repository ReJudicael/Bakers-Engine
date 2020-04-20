#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Collider.h"

namespace Resources
{
	namespace Loader
	{
		class ResourcesManager;
	}
}

namespace Core::Physics
{
	class SphereCollider :public Collider
	{
		float	m_extent{ 0.5f };
	public:
		SphereCollider() = default;

		SphereCollider(Resources::Loader::ResourcesManager* resources);

		/**
		 * Function inheritated from Collider,
		 * override for create a specific shape a box collider
		 * @param scene: the PhysX scene from the PhysicsScene
		 */
		virtual void CreateShape(physx::PxPhysics * physics) override;

		/**
		 * Set the half extent of the capsule collider
		 * @param halfExtent: the half extent we want to give to the shape
		 */
		void SetSphereHalfExtent(float halfExtent);

		/**
		 * Get the half extent of the capsule collider
		 * @return the half extent of the capsule collider
		 */
		float GetSphereHalfExtent();

		/**
		 * Draw the collider as a capsule
		 * @param cam: Camera to render to
		 * @param pos: position in global of the physics actor
		 * @param rot: rotation in global of the physics actor
		 */
		virtual void DrawCollider(Core::Datastructure::ICamera * cam, const Core::Maths::Vec3 & pos, const Core::Maths::Quat & rot) override;

		REGISTER_CLASS(Collider)
	};
}