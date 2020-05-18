#include <cmath>
#include "Collider.h"
#include "Vec3.hpp"
#include "PxPhysicsAPI.h"
#include "PxRigidDynamic.h"
#include "PhysicsScene.h"
#include "LoadResources.h"
#include "RootObject.hpp"
#include "EngineCore.h"
#include "DynamicMesh.h"


namespace Core::Physics
{
	RTTR_PLUGIN_REGISTRATION
	{
		registration::class_<Core::Physics::Collider>("Collider")
		.property("LocalPositon", &Core::Physics::Collider::GetLocalPosition, &Core::Physics::Collider::SetLocalPosition)
		.property("LocalRotation", &Core::Physics::Collider::GetLocalRotationEuler, &Core::Physics::Collider::SetLocalRotationEuler)
		.property("Mat: S D R", &Core::Physics::Collider::GetMaterial, &Core::Physics::Collider::SetMaterial)
		.property("Trigger", &Core::Physics::Collider::IsTrigger, &Core::Physics::Collider::Trigger)
		;
	}

	void Collider::OnInit()
	{
		Core::Datastructure::RootObject* root = GetRoot();

		root->GetEngine()->GetPhysicsScene()->CreatePhysicsShape(*this);
		root->GetEngine()->GetPhysicsScene()->CreateRigidStatic(m_pxRigidActor, m_pxShape, GetParent()->GetUpdatedTransform());
		m_pxRigidActor->userData = static_cast<void*>(dynamic_cast<Core::Datastructure::IComponent*>(
									dynamic_cast<Core::Physics::Collider*>(this)));

		Core::Datastructure::IRenderable::OnInit();
		Core::Datastructure::IComponent::OnInit();
	}

	bool Collider::OnStart()
	{
		m_IDFunctionSetTRS = GetParent()->SetAnEventTransformChange(std::bind(&Collider::SetPhysicsTransformParent, this));
		return Core::Datastructure::IRenderable::OnStart() && Core::Datastructure::IComponent::OnStart();

	}

	void Collider::OnCopy(IComponent* copyTo) const
	{

		Collider* phy = dynamic_cast<Collider*>(copyTo);

		phy->m_pxMaterial = m_pxMaterial;
		phy->m_pxRigidActor = m_pxRigidActor;
		phy->m_shader = m_shader;
		phy->m_model = m_model;
		phy->OnTriggerEnterEvent = OnTriggerEnterEvent;
		phy->OnTriggerExitEvent = OnTriggerExitEvent;
		phy->OnContactEvent = OnContactEvent;

		Core::Datastructure::IRenderable::OnCopy(copyTo);
		Core::Datastructure::IComponent::OnCopy(copyTo);

	}

	void Collider::DestroyRigidActor()
	{
		GetRoot()->GetEngine()->GetPhysicsScene()->RemoveActorFromPhysicsScene(m_pxRigidActor);
		DestroyShape();
		m_pxRigidActor->release();
		m_pxRigidActor = nullptr;
	}

	void Collider::OnDestroy()
	{
		Core::Datastructure::IRenderable::OnDestroy();
		Core::Datastructure::IComponent::OnDestroy();
		if (IsStarted())
		{
			DestroyRigidActor();
			GetParent()->DeleteAnEventTransformChange(m_IDFunctionSetTRS);
		}
	}

	void Collider::OnReset()
	{
		// TODO
		//Maybe To improve
		DestroyRigidActor();
		Core::Datastructure::IRenderable::OnReset();
		Core::Datastructure::IComponent::OnReset();
	}

	void Collider::InitRigidBody(Core::Physics::RigidBody* rigidBody, int& ID, physx::PxRigidDynamic*& pxRigidBody)
	{
		if (m_pxShape->getActor())
			m_pxRigidActor->detachShape(*m_pxShape);

		pxRigidBody = GetRoot()->GetEngine()->GetPhysicsScene()->CreateRigidDynamic(m_pxRigidActor, m_pxShape, GetParent()->GetUpdatedTransform());

		ID = GetParent()->SetAnEventTransformChange(
								std::bind(&Core::Physics::RigidBody::SetPhysicsTransformParent, rigidBody));
	}

	void Collider::SetLocalPosition(Core::Maths::Vec3 pos)
	{
		if (!m_pxShape)
			return;
		physx::PxTransform transform = m_pxShape->getLocalPose();
		transform.p = physx::PxVec3(pos.x, pos.y, pos.z);
		m_pxShape->setLocalPose(transform);
	}

	Core::Maths::Vec3 Collider::GetLocalPosition()
	{
		if (!m_pxShape)
			return{};
		physx::PxVec3 localPos = m_pxShape->getLocalPose().p;
		return { localPos.x, localPos.y, localPos.z };
	}

	void Collider::SetLocalRotationQuat(const Core::Maths::Quat& quat)
	{
		if (!m_pxShape)
			return;
		physx::PxTransform transform = m_pxShape->getLocalPose();
		transform.q = physx::PxQuat(quat.x, quat.y, quat.z, quat.w);
		m_pxShape->setLocalPose(transform);
	}

	Core::Maths::Quat Collider::GetLocalRotationQuat()
	{
		if (!m_pxShape)
			return{};

		physx::PxQuat localRot = m_pxShape->getLocalPose().q;
		return { localRot.w, localRot.x, localRot.y, localRot.z };
	}

	void Collider::SetLocalRotationEuler(Core::Maths::Vec3 euler)
	{
		if (!m_pxShape)
			return;
		physx::PxTransform transform = m_pxShape->getLocalPose();
		Core::Maths::Quat quat{ euler };
		transform.q = { quat.x, quat.y, quat.z, quat.w };
		m_pxShape->setLocalPose(transform);
	}

	Core::Maths::Vec3 Collider::GetLocalRotationEuler()
	{
		if (!m_pxShape)
			return{};
		physx::PxQuat localRot = m_pxShape->getLocalPose().q;
		Core::Maths::Quat quat{ localRot.w, localRot.x, localRot.y, localRot.z };
		return quat.ToEulerAngles();
	}

	void Collider::SetMaterial(Core::Maths::Vec3 mat)
	{
		if (!m_pxShape)
			return;
		m_pxMaterial->setStaticFriction(mat.x);
		m_pxMaterial->setDynamicFriction(mat.y);
		m_pxMaterial->setRestitution(mat.z);
	}

	Core::Maths::Vec3 Collider::GetMaterial()
	{
		if (!m_pxShape)
			return {};
		return { m_pxMaterial->getStaticFriction(), m_pxMaterial->getDynamicFriction(), m_pxMaterial->getRestitution() };
	}

	void Collider::Trigger(bool trigger)
	{
		if (!m_pxShape)
			return;
		if (trigger)
			TriggerCollider();
		else
			SimulationCollider();
	}

	bool Collider::IsTrigger()
	{
		if (!m_pxShape)
			return false;
		return m_pxShape->getFlags().isSet(physx::PxShapeFlag::eTRIGGER_SHAPE);
	}

	void Collider::TriggerCollider()
	{
		if (!m_pxShape)
			return;
		m_pxShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
		m_pxShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
	}

	void Collider::SimulationCollider()
	{
		if (!m_pxShape)
			return;
		m_pxShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
		m_pxShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
	}

	void Collider::SetRaycastFilter(const EFilterRaycast& filter)
	{
		if (!m_pxShape)
			return;
		physx::PxFilterData filterData;
		filterData.word0 = static_cast<physx::PxU32>(filter);
		m_pxShape->setQueryFilterData(filterData);
	}

	EFilterRaycast Collider::GetRaycastFilter()
	{
		if (!m_pxShape)
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

	void Collider::SetPhysicsTransformParent()
	{
		if (m_pxRigidActor == nullptr)
			return;

		Maths::Vec3 vec = GetParent()->GetGlobalPos();
		physx::PxVec3 position = physx::PxVec3{ vec.x, vec.y, vec.z };
		Maths::Quat quat = GetParent()->GetGlobalRot();
		physx::PxQuat rotation = physx::PxQuat{ quat.x, quat.y, quat.z, quat.w };

		m_pxRigidActor->setGlobalPose(physx::PxTransform(position, rotation));
	}

	Collider::~Collider()
	{
		DestroyShape();
	}

}