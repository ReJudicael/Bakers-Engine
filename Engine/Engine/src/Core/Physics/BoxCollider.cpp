#include "BoxCollider.h"
#include "Collider.h"
#include "Vec3.hpp"
#include "PxPhysics.h"

namespace Core::Physics
{
	RTTR_PLUGIN_REGISTRATION
	{
		registration::class_<Core::Physics::BoxCollider>("Collider")
		.property("Box Half Extent", &Core::Physics::BoxCollider::GetBoxHalfExtent ,&Core::Physics::BoxCollider::SetBoxHalfExtent)
		;
	}

	void BoxCollider::CreateShape(physx::PxPhysics* physics)
	{
		physx::PxVec3 extent = physx::PxVec3(m_extent.x, m_extent.y, m_extent.z);
		physx::PxVec3 localPosition = physx::PxVec3(0.f, 0.f, 0.f);

		m_pxMaterial = physics->createMaterial(1.5f, 1.5f, 0.0f);
		m_pxShape = physics->createShape(physx::PxBoxGeometry(extent), *m_pxMaterial, true);
		m_pxShape->setLocalPose(physx::PxTransform(localPosition));
	}

	void BoxCollider::SetBoxHalfExtent(Core::Maths::Vec3 halfExtent)
	{
		m_extent = halfExtent;
		if(m_pxShape != nullptr)
			m_pxShape->setGeometry(physx::PxBoxGeometry(physx::PxVec3{ halfExtent.x, halfExtent.y, halfExtent.z }));
	}
	
	Core::Maths::Vec3 BoxCollider::GetBoxHalfExtent()
	{
		if (m_pxShape != nullptr)
		{
			physx::PxBoxGeometry box;
			if (m_pxShape->getBoxGeometry(box))
				return { box.halfExtents.x, box.halfExtents.y, box.halfExtents.y };

			return Core::Maths::Vec3();
		}

	}
}