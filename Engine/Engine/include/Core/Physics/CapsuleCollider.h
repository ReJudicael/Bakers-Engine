#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ComponentBase.h"
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

	BAKERS_API_CLASS CapsuleCollider : public Core::Datastructure::ComponentBase, public virtual Collider
	{
	protected:

		virtual void StartCopy(IComponent*& copyTo) const override;

		virtual void OnCopy(IComponent* copyTo) const override;

		/**
		 * Function inheritated from IComponent,
		 * override for delete the collider
		 */
		virtual void OnDestroy() override;

		virtual void OnReset() override;

		virtual void SetToDefault() override;

		virtual bool OnStart() override;

	public:

		CapsuleCollider() = default;

		virtual void OnDraw(Core::Datastructure::ICamera* cam) override;

		virtual void OnInit() override;

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
		Core::Maths::Vec2 GetCapsuleHalfExtent() const;

		/**
		 * Draw the collider as a capsule
		 * @param cam: Camera to render to
		 * @param pos: position in global of the physics actor
		 * @param rot: rotation in global of the physics actor
		 */
		virtual void DrawCollider(Core::Datastructure::ICamera* cam, const Core::Maths::Vec3& pos, const Core::Maths::Quat& rot) override {};

		REGISTER_CLASS(Core::Datastructure::ComponentBase, Collider)
	};
}

