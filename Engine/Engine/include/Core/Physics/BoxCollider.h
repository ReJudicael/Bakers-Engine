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
	BAKERS_API_CLASS BoxCollider : public Collider
	{
		Core::Maths::Vec3	m_extent{ 0.5f,0.5f,0.5f };
	public :
		BoxCollider() = default;

		BoxCollider(Resources::Loader::ResourcesManager* resources);

		/**
		 * Function inheritated from Collider,
		 * override for create a specific shape a box collider
		 * @param scene: the PhysX scene from the PhysicsScene
		 */
		virtual void CreateShape(physx::PxPhysics* physics) override;

		/**
		 * Set the half extent of the box collider
		 * @param halfExtent: the half extent we want to give to the shape
		 */
		void SetBoxHalfExtent(Core::Maths::Vec3 halfExtent);

		/**
		 * Get the half extent of the capsule collider
		 * @return the half extent of the box collider
		 */
		Core::Maths::Vec3 GetBoxHalfExtent();

		/**
		 * Draw the collider as a box
		 * @param view: View matrix
		 * @param proj: Projection matrix
		 * @param pos: position in global of the physics actor
		 * @param rot: rotation in global of the physics actor
		 */
		virtual void DrawCollider(const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj, const Core::Maths::Vec3& pos, const Core::Maths::Quat& rot) override;

		REGISTER_CLASS(Collider)
	};
}
