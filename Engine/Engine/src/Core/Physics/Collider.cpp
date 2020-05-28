#include <cmath>
#include "Collider.h"
#include "Vec3.hpp"
#include "PxPhysicsAPI.h"
#include "PxRigidDynamic.h"
#include "PhysicsScene.h"
#include "LoadResources.h"
#include "RootObject.hpp"
#include "EngineCore.h"
#include "Maths.hpp"
#include "RigidBody.h"


namespace Core::Physics
{
	RTTR_PLUGIN_REGISTRATION
	{
		ZoneScopedN("Registering RTTR")
		registration::class_<Core::Physics::Collider>("Collider")
		.enumeration<Core::Physics::EFilterRaycast>("Layout")
			(
			value("GROUPE1", Core::Physics::EFilterRaycast::GROUPE1),
			value("GROUPE2", Core::Physics::EFilterRaycast::GROUPE2),
			value("GROUPE3", Core::Physics::EFilterRaycast::GROUPE3),
			value("GROUPE4", Core::Physics::EFilterRaycast::GROUPE4)
			)
		.property("LocalPositon", &Core::Physics::Collider::GetLocalPosition, &Core::Physics::Collider::SetLocalPosition)
		.property("LocalRotation", &Core::Physics::Collider::GetLocalRotationEuler, &Core::Physics::Collider::SetLocalRotationEuler)
		.property("Mat: S D R", &Core::Physics::Collider::GetMaterial, &Core::Physics::Collider::SetMaterial)
		.property("Trigger", &Core::Physics::Collider::IsTrigger, &Core::Physics::Collider::Trigger)
		;
	}

	void Collider::OnInit()
	{
		Core::Datastructure::RootObject* root = GetRoot();

		// create the shape of the collider
		root->GetEngine()->GetPhysicsScene()->CreatePhysicsShape(*this);

		CreateActor();

		Core::Datastructure::IComponent::OnInit();
	}

	bool Collider::OnStart()
	{
		return Core::Datastructure::IComponent::OnStart();
	}

	void Collider::OnCopy(IComponent* copyTo) const
	{

		Collider* col = dynamic_cast<Collider*>(copyTo);

		col->m_tmpColliderSave = SaveCollider();

		Core::Datastructure::IComponent::OnCopy(copyTo);

	}

	ColliderSave* Collider::SaveCollider() const
	{
		if (!m_pxRigidActor && !m_pxShape && !m_pxMaterial)
			return nullptr;
		ColliderSave* colliderSave = new ColliderSave();

		colliderSave->isTrigger = IsTrigger();
		colliderSave->localPosition = GetLocalPosition();
		colliderSave->localRotation = GetLocalRotationEuler();
		colliderSave->physicsMaterial = GetMaterial();
		colliderSave->raycastFilter = GetRaycastFilter();

		return colliderSave;
	}

	void Collider::DestroyRigidActor()
	{
		// detach the actor from the physics scene
		GetRoot()->GetEngine()->GetPhysicsScene()->RemoveActorFromPhysicsScene(m_pxRigidActor);
		// detroy the shape
		DestroyShape();
		// release teh actor if it's possible
		if(m_pxRigidActor->isReleasable())
			m_pxRigidActor->release();

		m_pxRigidActor = nullptr;
	}

	void Collider::OnDestroy()
	{
		Core::Datastructure::IComponent::OnDestroy();
		DestroyRigidActor();
		std::list<Core::Physics::RigidBody*> colliders;
		GetParent()->GetComponentsOfBaseTypeInObject(colliders);
		if (colliders.size() > 0)
			(*colliders.begin())->ToNull();
		GetParent()->DeleteAnEventTransformChange(m_IDFunctionSetTRS);
	}

	void Collider::OnReset()
	{
		Core::Datastructure::IComponent::OnReset();
	}

	void Collider::SetToDefault()
	{
		SetLocalPosition({ 0.f, 0.f, 0.f });
		SetLocalRotationQuat({ 1.f, 0.f, 0.f, 0.f });
		Trigger(false);
		SetMaterial({ 1.f, 1.f, 0.f });
	}

	void Collider::DetachShape()
	{
		// detach the collider shape for his actor if he have one
		if (m_pxShape->getActor())
			m_pxRigidActor->detachShape(*m_pxShape);

		if (m_pxRigidActor && m_pxRigidActor->getScene())
		{
			// detach the actor from the physics scene
			GetRoot()->GetEngine()->GetPhysicsScene()->RemoveActorFromPhysicsScene(m_pxRigidActor);

			// release the collider rigid actor
			if (m_pxRigidActor->isReleasable())
				m_pxRigidActor->release();
		}
	}

	void Collider::CreateActor()
	{
		Core::Datastructure::RootObject* root = GetRoot();

		DetachShape();

		// create the collider RigidActor as a StaticActor
		root->GetEngine()->GetPhysicsScene()->CreateRigidStatic(m_pxRigidActor, m_pxShape, GetParent()->GetUpdatedTransform());
		// init the user data of the actor
		m_pxRigidActor->userData = static_cast<void*>(dynamic_cast<Core::Datastructure::IComponent*>(
			dynamic_cast<Core::Physics::Collider*>(this)));

		m_IDFunctionSetTRS = GetParent()->SetAnEventTransformChange(std::bind(&Collider::SetPhysicsTransformParent, this));
	}

	void Collider::InitRigidBody(Core::Physics::RigidBody* rigidBody, int& ID, physx::PxRigidDynamic*& pxRigidBody)
	{
		DetachShape();

		// recreate the collider rigid actor as a dynamic actor
		pxRigidBody = GetRoot()->GetEngine()->GetPhysicsScene()->CreateRigidDynamic(m_pxRigidActor, m_pxShape, GetParent()->GetUpdatedTransform());

		// init the user data of the actor
		m_pxRigidActor->userData = static_cast<void*>(dynamic_cast<Core::Datastructure::IComponent*>(
									dynamic_cast<Core::Physics::Collider*>(this)));

		ID = GetParent()->SetAnEventTransformChange(
								std::bind(&Core::Physics::RigidBody::SetPhysicsTransformParent, rigidBody));
		// Delete the function who set the position of the actor if the object move
		GetParent()->DeleteAnEventTransformChange(m_IDFunctionSetTRS);

		// Init the actor as his save
		rigidBody->InitPhysic();
	}

	void Collider::SetLocalPosition(Core::Maths::Vec3 pos)
	{
		// if there is already a shape put the value in the shape
		// if the object isn't going to be destroyed save the value
		if (m_pxShape)
		{
			physx::PxTransform transform = m_pxShape->getLocalPose();
			transform.p = physx::PxVec3(pos.x, pos.y, pos.z);
			m_pxShape->setLocalPose(transform);
		}
		else if (!IsDestroyed())
		{
			if (!m_tmpColliderSave)
				m_tmpColliderSave = new ColliderSave();
			m_tmpColliderSave->localPosition = pos;
		}
	}

	Core::Maths::Vec3 Collider::GetLocalPosition() const
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

	Core::Maths::Quat Collider::GetLocalRotationQuat() const
	{
		if (!m_pxShape)
			return{};

		physx::PxQuat localRot = m_pxShape->getLocalPose().q;
		return { localRot.w, localRot.x, localRot.y, localRot.z };
	}

	void Collider::SetLocalRotationEuler(Core::Maths::Vec3 euler)
	{
		// if there is already a shape put the value in the shape
		// if the object isn't going to be destroyed save the value
		if (m_pxShape)
		{
			physx::PxTransform transform = m_pxShape->getLocalPose();
			Core::Maths::Quat quat{ Core::Maths::ToRadiansf(euler.x), 
									Core::Maths::ToRadiansf(euler.y), 
									Core::Maths::ToRadiansf(euler.z) };

			transform.q = physx::PxQuat{ quat.x, quat.y, quat.z, quat.w };
			m_pxShape->setLocalPose(transform);
		}
		else if (!IsDestroyed())
		{
			if (!m_tmpColliderSave)
				m_tmpColliderSave = new ColliderSave();
			m_tmpColliderSave->localRotation = {euler};
		}
	}

	Core::Maths::Vec3 Collider::GetLocalRotationEuler() const
	{
		if (!m_pxShape)
			return{};
		physx::PxQuat localRot = m_pxShape->getLocalPose().q;
		Core::Maths::Quat quat{ localRot.w, localRot.x, localRot.y, localRot.z };
		return	{	Core::Maths::ToDegreesf(quat.ToEulerAngles().x),
					Core::Maths::ToDegreesf(quat.ToEulerAngles().y), 
					Core::Maths::ToDegreesf(quat.ToEulerAngles().z) };
	}

	void Collider::SetMaterial(Core::Maths::Vec3 mat)
	{
		// if there is already a shape put the value in the shape
		// if the object isn't going to be destroyed save the value
		if (m_pxShape)
		{
			m_pxMaterial->setStaticFriction(mat.x);
			m_pxMaterial->setDynamicFriction(mat.y);
			m_pxMaterial->setRestitution(mat.z);
		}
		else if (!IsDestroyed())
		{
			if (!m_tmpColliderSave)
				m_tmpColliderSave = new ColliderSave();
			m_tmpColliderSave->physicsMaterial = { mat };
		}
	}

	Core::Maths::Vec3 Collider::GetMaterial() const
	{
		if (!m_pxShape)
			return {};
		return { m_pxMaterial->getStaticFriction(), m_pxMaterial->getDynamicFriction(), m_pxMaterial->getRestitution() };
	}

	void Collider::Trigger(bool trigger)
	{
		// if there is already a shape put the value in the shape
		// if the object isn't going to be destroyed save the value
		if (m_pxShape)
		{
			if (trigger)
				TriggerCollider();
			else
				SimulationCollider();
		}
		else if (!IsDestroyed())
		{
			if (!m_tmpColliderSave)
				m_tmpColliderSave = new ColliderSave();
			m_tmpColliderSave->isTrigger = trigger;
		}
	}

	bool Collider::IsTrigger() const
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
		// if there is already a shape put the value in the shape
		// if the object isn't going to be destroyed save the value
		if (m_pxShape)
		{
			physx::PxFilterData filterData;
			filterData.word0 = static_cast<physx::PxU32>(filter);
			m_pxShape->setQueryFilterData(filterData);
		}
		else if (!IsDestroyed())
		{
			if (!m_tmpColliderSave)
				m_tmpColliderSave = new ColliderSave();
			m_tmpColliderSave->raycastFilter = filter;
		}
	}

	EFilterRaycast Collider::GetRaycastFilter() const
	{
		if (!m_pxShape)
			return EFilterRaycast::GROUPE1;
		physx::PxFilterData filterData{ m_pxShape->getQueryFilterData()};

		return static_cast<EFilterRaycast>(filterData.word0);
	}

	void Collider::DestroyShape()
	{
		if (m_pxShape)
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
		if (m_tmpColliderSave)
			delete m_tmpColliderSave;
	}

}