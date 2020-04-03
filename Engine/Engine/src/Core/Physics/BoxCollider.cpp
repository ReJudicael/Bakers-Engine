#include "BoxCollider.h"
#include "Collider.h"
#include "Vec3.hpp"
#include "PxPhysics.h"

namespace Core::Physics
{
	void BoxCollider::CreateShape(physx::PxPhysics* physics)
	{
		physx::PxVec3 extent = physx::PxVec3(m_extent.x, m_extent.y, m_extent.z);
		physx::PxVec3 localPosition = physx::PxVec3(0.f, 0.f, 0.f);
		//physx::PxVec3 material = physx::PxVec3(m_localPosition.x, m_localPosition.y, m_localPosition.z);
		m_pxMaterial = physics->createMaterial(0.5f, 0.5f, 0.1f);
		m_pxShape = physics->createShape(physx::PxBoxGeometry(extent), *m_pxMaterial, true);
		m_pxShape->setLocalPose(physx::PxTransform(localPosition));
	}

	void BoxCollider::SetBoxHalfExtent(const Core::Maths::Vec3& halfExtent)
	{
		m_extent = halfExtent;
		if(m_pxShape != nullptr)
			m_pxShape->setGeometry(physx::PxBoxGeometry(physx::PxVec3{ halfExtent.x, halfExtent.y, halfExtent.z }));
	}
}