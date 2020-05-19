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

	BAKERS_API_CLASS BoxCollider : public Core::Datastructure::ComponentBase, public virtual Collider
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

		virtual bool OnStart() override;

	public :
		BoxCollider() = default;

		virtual void OnDraw(Core::Datastructure::ICamera* cam) override;

		virtual void OnInit() override;

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
		Core::Maths::Vec3 GetBoxHalfExtent() const;

		/**
		 * Draw the collider as a box
		 * @param cam: Camera to render to
		 * @param pos: position in global of the physics actor
		 * @param rot: rotation in global of the physics actor
		 */
		virtual void DrawCollider(Core::Datastructure::ICamera* cam, const Core::Maths::Vec3& pos, const Core::Maths::Quat& rot) override {};

		REGISTER_CLASS(Core::Datastructure::ComponentBase, Collider)
	};
}
