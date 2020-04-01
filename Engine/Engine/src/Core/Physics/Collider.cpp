#include <cmath>
#include "Collider.h"
#include "Vec3.hpp"
#include "PxPhysicsAPI.h"


namespace Core::Physics
{

	void Collider::initCube(const Core::Maths::Vec3& extent, const Core::Maths::Vec3& physicsMaterial ,const Core::Maths::Vec3& localPosition)
	{
		m_extent = extent;
		m_physicsMaterial = physicsMaterial;
		m_localPosition = localPosition;
	}
	void Collider::createCuceShape(physx::PxPhysics* physics)
	{
		physx::PxVec3 extent = physx::PxVec3(std::abs(m_extent.x), std::abs(m_extent.y), std::abs(m_extent.z));
		physx::PxVec3 localPosition = physx::PxVec3(m_localPosition.x, m_localPosition.y, m_localPosition.z);
		physx::PxVec3 material = physx::PxVec3(m_localPosition.x, m_localPosition.y, m_localPosition.z);
		m_pxMaterial = physics->createMaterial(material.x, material.y, material.z);
		m_pxShape = physics->createShape(physx::PxBoxGeometry(extent), *m_pxMaterial,true);
		m_pxShape->setLocalPose(physx::PxTransform(localPosition));
	}

}