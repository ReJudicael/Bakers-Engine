#include <cmath>
#include "Collider.h"
#include "Vec3.hpp"
#include "PxPhysicsAPI.h"


namespace Core::Physics
{

	void Collider::SetLocalPosition(const Core::Maths::Vec3& vec)
	{
		physx::PxTransform transform = m_pxShape->getLocalPose();
		transform.p = physx::PxVec3(vec.x, vec.y, vec.z);
		m_pxShape->setLocalPose(transform);
	}

	void Collider::SetLocalRotation(const Core::Maths::Quat& quat)
	{
		physx::PxTransform transform = m_pxShape->getLocalPose();
		transform.q = physx::PxQuat(quat.x, quat.y, quat.z, quat.w);
		m_pxShape->setLocalPose(transform);
	}

	void Collider::DestroyShape()
	{
		m_pxShape->release();
		m_pxMaterial->release();
	}

	Collider::~Collider()
	{
		DestroyShape();
	}

}