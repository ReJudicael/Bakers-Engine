#include "DynamicMesh.h"
#include "PxScene.h"
#include "PxRigidStatic.h"
#include "PxPhysics.h"

#include "RootObject.hpp"
#include "PhysicsScene.h"
#include "EngineCore.h"
#include "Maths.hpp"
#include "CapsuleCollider.h"

namespace Core
{
	namespace Physics
	{
		RTTR_PLUGIN_REGISTRATION
		{
			registration::class_<Core::Physics::RigidBody>("Rigid Body")
			.constructor()
			.property("Velocity", &Core::Physics::RigidBody::GetVelocity, &Core::Physics::RigidBody::SetLinearVelocity)
			.property("Mass", &Core::Physics::RigidBody::GetMass, &Core::Physics::RigidBody::SetMass)
			.property("Rotation XLock", &Core::Physics::RigidBody::GetPhysicsLockXRotation,
						&Core::Physics::RigidBody::SetPhysicsLockXRotation)
			.property("Rotation YLock", &Core::Physics::RigidBody::GetPhysicsLockYRotation,
						&Core::Physics::RigidBody::SetPhysicsLockYRotation)
			.property("Rotation ZLock", &Core::Physics::RigidBody::GetPhysicsLockZRotation,
						&Core::Physics::RigidBody::SetPhysicsLockZRotation)
			;
		}

		void RigidBody::OnInit()
		{
			std::list<Core::Physics::Collider*> colliders;
			GetParent()->GetComponentsOfBaseTypeInObject(colliders);

			if (colliders.size() > 0)
				(*colliders.begin())->InitRigidBody(this, m_IDFunctionSetTRS, m_pxRigidBody);

			Core::Datastructure::IUpdatable::OnInit();
		}

		bool RigidBody::OnStart()
		{
			if (!m_pxRigidBody)
				OnInit();
			return IUpdatable::OnStart() && m_pxRigidBody;
		}

		void RigidBody::StartCopy(IComponent*& copyTo) const
		{
			copyTo = new RigidBody();
			OnCopy(copyTo);

		}

		void RigidBody::OnCopy(IComponent* copyTo) const
		{
			ComponentBase::OnCopy(copyTo);
			IUpdatable::OnCopy(copyTo);

			RigidBody* phy = dynamic_cast<RigidBody*>(copyTo);

			phy->m_pxRigidBody = m_pxRigidBody;
			phy->m_BodyChangeGlobalPos = m_BodyChangeGlobalPos;
			phy->m_IDFunctionSetTRS = m_IDFunctionSetTRS;

		}

		void RigidBody::DestroyDynamicMesh()
		{
			if (m_pxRigidBody)
			{
				GetRoot()->GetEngine()->GetPhysicsScene()->RemoveActorFromPhysicsScene(m_pxRigidBody);
				m_pxRigidBody->release();
				m_pxRigidBody = nullptr;
			}
		}

		void RigidBody::OnDestroy()
		{
			ComponentBase::OnDestroy();
			IUpdatable::OnDestroy();
			if (IsStarted())
			{
				//DestroyDynamicMesh();
				GetParent()->DeleteAnEventTransformChange(m_IDFunctionSetTRS);
				GetParent()->RemoveEventTransformChange();
			}
		}

		void RigidBody::OnReset()
		{
			// TODO
			//Maybe To improve
			DestroyDynamicMesh();
			ComponentBase::OnReset();
			IUpdatable::OnReset();
		}

		/*void RigidBody::CreateActor(physx::PxPhysics* physics, physx::PxScene* scene)
		{
			Maths::Vec3 vec = GetParent()->GetGlobalPos();
			physx::PxVec3 position = physx::PxVec3{ vec.x, vec.y, vec.z };
			Maths::Quat quat = GetParent()->GetGlobalRot();
			physx::PxQuat rotation = physx::PxQuat{ quat.x, quat.y, quat.z, quat.w };
			m_dynamicMesh = physics->createRigidDynamic(physx::PxTransform(position, rotation));
			m_dynamicMesh->attachShape(*m_collider->GetShape());
			
			m_dynamicMesh->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, true);
			
			// usefull if we don't want to do the update every time if the rigid doesn't move
			//m_dynamicMesh->setActorFlag(physx::PxActorFlag::eSEND_SLEEP_NOTIFIES);
			
			m_dynamicMesh->userData = static_cast<void*>(dynamic_cast<Core::Datastructure::IComponent*>(dynamic_cast<Core::Datastructure::IPhysics*>(this)));

			scene->addActor(*m_dynamicMesh);
			PhysicsLockRotation(true, false, true);
		}*/

		void RigidBody::InitPhysic(physx::PxShape* shape)
		{
			
		}

		void RigidBody::OnUpdate(float deltaTime)
		{
			if (!m_pxRigidBody)
			{
				GetParent()->DeleteAnEventTransformChange(m_IDFunctionSetTRS);
			}

			physx::PxVec3 pos = m_pxRigidBody->getGlobalPose().p;
			physx::PxQuat rot = m_pxRigidBody->getGlobalPose().q;

			m_BodyChangeGlobalPos = true;
			GetParent()->SetGlobalPos({ pos.x, pos.y, pos.z });
			m_BodyChangeGlobalPos = true;
			GetParent()->SetGlobalRot({ rot.w, rot.x, rot.y, rot.z });
			//float y = m_dynamicMesh->getLinearVelocity().y;

		}

		void RigidBody::SetPhysicsTransformParent()
		{
			if (m_pxRigidBody == nullptr || m_BodyChangeGlobalPos)
			{
				m_BodyChangeGlobalPos = false;
				return;
			}

			Maths::Vec3 vec = GetParent()->GetGlobalPos();
			physx::PxVec3 position = physx::PxVec3{ vec.x, vec.y, vec.z };
			Maths::Quat quat = GetParent()->GetGlobalRot();
			physx::PxQuat rotation = physx::PxQuat{ quat.x, quat.y, quat.z, quat.w };

			m_pxRigidBody->setGlobalPose(physx::PxTransform(position, rotation));

			ClearForces();
		}

		void RigidBody::SetLinearVelocity(Core::Maths::Vec3 newVelocity)
		{
			if (m_pxRigidBody == nullptr)
				return;
			m_pxRigidBody->setLinearVelocity({ newVelocity.x, newVelocity.y, newVelocity.z });
		}

		Core::Maths::Vec3 RigidBody::GetVelocity()
		{
			if (m_pxRigidBody == nullptr)
				return { 0.f, 0.f, 0.f };
			physx::PxVec3 vec{ m_pxRigidBody->getLinearVelocity() };
			return { vec.x, vec.y, vec.z };
		}

		void RigidBody::AddVelocity(const Core::Maths::Vec3 vector)
		{
			m_pxRigidBody->addForce({ vector.x,vector.y,vector.z }, physx::PxForceMode::eVELOCITY_CHANGE);
		}

		void RigidBody::SetMass(const float mass)
		{
			if (m_pxRigidBody == nullptr)
				return;
			m_pxRigidBody->setMass(static_cast<physx::PxReal>(mass));
		}

		float RigidBody::GetMass()
		{
			if (m_pxRigidBody == nullptr)
				return 0;
			return m_pxRigidBody->getMass();
		}

		void RigidBody::ClearForces()
		{
			if (m_pxRigidBody == nullptr)
				return;
			m_pxRigidBody->clearForce();
			m_pxRigidBody->clearTorque();
		}

		void RigidBody::PhysicsLockRotation(bool Axisx, bool Axisy, bool Axisz)
		{
			SetPhysicsLockXRotation(Axisx);
			SetPhysicsLockYRotation(Axisy);
			SetPhysicsLockZRotation(Axisz);
		}


		void RigidBody::SetPhysicsLockXRotation(bool Axisx)
		{
			if (m_pxRigidBody == nullptr)
				return;
			m_pxRigidBody->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, Axisx);
		}

		bool RigidBody::GetPhysicsLockXRotation()
		{
			if (m_pxRigidBody == nullptr)
				return false;
			return m_pxRigidBody->getRigidDynamicLockFlags().isSet(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X);
		}

		void RigidBody::SetPhysicsLockYRotation(bool Axisy)
		{
			if (m_pxRigidBody == nullptr)
				return;
			m_pxRigidBody->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, Axisy);
		}

		bool RigidBody::GetPhysicsLockYRotation()
		{
			if (m_pxRigidBody == nullptr)
				return false;
			return m_pxRigidBody->getRigidDynamicLockFlags().isSet(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y);
		}

		void RigidBody::SetPhysicsLockZRotation(bool Axisz)
		{
			if (m_pxRigidBody == nullptr)
				return;
			m_pxRigidBody->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, Axisz);
		}

		bool RigidBody::GetPhysicsLockZRotation()
		{
			if (m_pxRigidBody == nullptr)
				return false;
			return m_pxRigidBody->getRigidDynamicLockFlags().isSet(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
		}
	}
}