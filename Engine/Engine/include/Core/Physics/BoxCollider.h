#pragma once
#include "Collider.h"

namespace Core::Physics
{
	class BoxCollider : public Collider
	{
		Core::Maths::Vec3	m_extent{ 0.5f,0.5f,0.5f };
		Core::Maths::Vec3	m_localPosition{ 0.f,0.f,0.f };
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
		void SetBoxHalfExtent(const Core::Maths::Vec3& halfExtent);
	};
}
