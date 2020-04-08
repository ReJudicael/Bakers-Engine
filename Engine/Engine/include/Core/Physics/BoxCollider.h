#pragma once
#include "Collider.h"

namespace Core::Physics
{
	BAKERS_API_CLASS BoxCollider : public Collider
	{
		Core::Maths::Vec3	m_extent{ 0.5f,0.5f,0.5f };
	public :
		BoxCollider() = default;
		/**
		 * Function inheritated from Collider,
		 * override for create a specific shape a box collider
		 * @param scene: the PhysX scene from the PhysicsScene
		 */
		virtual void CreateShape(physx::PxPhysics* physics) override;

		/**
		 * Set the half extent of the collider
		 * @param halfExtent: the half extent we want to give to the shape
		 */
		void SetBoxHalfExtent(Core::Maths::Vec3 halfExtent);


		Core::Maths::Vec3 GetBoxHalfExtent();

		REGISTER_CLASS(Collider)
	};
}
