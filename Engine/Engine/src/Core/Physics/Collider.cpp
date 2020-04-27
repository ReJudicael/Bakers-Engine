#include <cmath>
#include "Collider.h"
#include "Vec3.hpp"
#include "PxPhysicsAPI.h"
#include "PhysicsScene.h"
#include "LoadResources.h"


namespace Core::Physics
{
	RTTR_PLUGIN_REGISTRATION
	{
		registration::class_<Core::Physics::Collider>("Collider")
		.property("LocalPositon", &Core::Physics::Collider::GetLocalPosition, &Core::Physics::Collider::SetLocalPosition)
		.property("LocalRotation", &Core::Physics::Collider::GetLocalRotationEuler, &Core::Physics::Collider::SetLocalRotationEuler)
		.property("Mat: Static, Dynamic, Restitution", &Core::Physics::Collider::GetMaterial, &Core::Physics::Collider::SetMaterial)
		.property("Trigger", &Core::Physics::Collider::IsTrigger, &Core::Physics::Collider::Trigger)
		;
	}

	Collider::Collider(Resources::Loader::ResourcesManager* resources):
		m_shader{resources->GetShader("Wireframe")}
	{}

	void Collider::SetLocalPosition(Core::Maths::Vec3 pos)
	{
		physx::PxTransform transform = m_pxShape->getLocalPose();
		transform.p = physx::PxVec3(pos.x, pos.y, pos.z);
		m_pxShape->setLocalPose(transform);
	}

	Core::Maths::Vec3 Collider::GetLocalPosition()
	{
		physx::PxVec3 localPos = m_pxShape->getLocalPose().p;
		return { localPos.x, localPos.y, localPos.z };
	}

	void Collider::SetLocalRotationQuat(const Core::Maths::Quat& quat)
	{
		physx::PxTransform transform = m_pxShape->getLocalPose();
		transform.q = physx::PxQuat(quat.x, quat.y, quat.z, quat.w);
		m_pxShape->setLocalPose(transform);
	}

	Core::Maths::Quat Collider::GetLocalRotationQuat()
	{
		physx::PxQuat localRot = m_pxShape->getLocalPose().q;
		return { localRot.w, localRot.x, localRot.y, localRot.z };
	}

	void Collider::SetLocalRotationEuler(Core::Maths::Vec3 euler)
	{
		physx::PxTransform transform = m_pxShape->getLocalPose();
		Core::Maths::Quat quat{ euler };
		transform.q = { quat.x, quat.y, quat.z, quat.w };
		m_pxShape->setLocalPose(transform);
	}

	Core::Maths::Vec3 Collider::GetLocalRotationEuler()
	{
		physx::PxQuat localRot = m_pxShape->getLocalPose().q;
		Core::Maths::Quat quat{ localRot.w, localRot.x, localRot.y, localRot.z };
		return quat.ToEulerAngles();
	}

	void Collider::SetMaterial(Core::Maths::Vec3 mat)
	{
		m_pxMaterial->setStaticFriction(mat.x);
		m_pxMaterial->setDynamicFriction(mat.y);
		m_pxMaterial->setRestitution(mat.z);
	}

	Core::Maths::Vec3 Collider::GetMaterial()
	{
		return { m_pxMaterial->getStaticFriction(), m_pxMaterial->getDynamicFriction(), m_pxMaterial->getRestitution() };
	}

	void Collider::Trigger(bool trigger)
	{
		if (trigger)
			TriggerCollider();
		else
			SimulationCollider();
	}

	bool Collider::IsTrigger()
	{
		return m_pxShape->getFlags().isSet(physx::PxShapeFlag::eTRIGGER_SHAPE);
	}

	void Collider::TriggerCollider()
	{
		m_pxShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
		m_pxShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
	}

	void Collider::SimulationCollider()
	{
		m_pxShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
		m_pxShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
	}

	void Collider::SetRaycastFilter(const EFilterRaycast& filter)
	{
		if (m_pxShape == nullptr)
			return;
		physx::PxFilterData filterData;
		filterData.word0 = static_cast<physx::PxU32>(filter);
		m_pxShape->setQueryFilterData(filterData);
	}

	EFilterRaycast Collider::GetRaycastFilter()
	{
		if (m_pxShape == nullptr)
			return EFilterRaycast::GROUPE1;
		physx::PxFilterData filterData{ m_pxShape->getQueryFilterData()};

		return static_cast<EFilterRaycast>(filterData.word0);
	}

	void Collider::DestroyShape()
	{
		if (m_pxShape == nullptr)
			return;
		m_pxShape->release();
		m_pxMaterial->release();
	}

	Collider::~Collider()
	{
		DestroyShape();
	}

}