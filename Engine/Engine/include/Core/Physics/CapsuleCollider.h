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

	class CapsuleCollider : public Collider
	{
		Core::Maths::Vec2	m_extent{ 0.5f,1.f };
	public:

		CapsuleCollider() = default;

		CapsuleCollider(Resources::Loader::ResourcesManager* resources);

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
		void SetCapsuleHalfExtent(Core::Maths::Vec2 halfExtent);

		/**
		 * Get the half extent of the capsule collider
		 * @return the half extent of the capsule collider
		 */
		Core::Maths::Vec2 GetCapsuleHalfExtent();

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

