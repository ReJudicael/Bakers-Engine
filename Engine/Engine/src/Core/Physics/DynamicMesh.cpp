#include "DynamicMesh.h"
#include "PxScene.h"
#include "PxRigidStatic.h"
#include "PxPhysics.h"

#include "RootObject.hpp"
#include "PhysicsScene.h"
#include "EngineCore.h"
#include "Maths.hpp"

namespace Core
{
	namespace Physics
	{
		RTTR_PLUGIN_REGISTRATION
		{
			registration::class_<Core::Physics::DynamicMesh>("DynamicMesh")
			.constructor()
			.constructor<Collider*>()
			.property("Velocity", &Core::Physics::DynamicMesh::GetVelocity, &Core::Physics::DynamicMesh::SetLinearVelocity)
			.property("Mass", &Core::Physics::DynamicMesh::GetMass, &Core::Physics::DynamicMesh::SetMass)
			.property("RotationLockX", &Core::Physics::DynamicMesh::GetPhysicsLockXRotation, 
						&Core::Physics::DynamicMesh::SetPhysicsLockXRotation)
			.property("RotationLockY", &Core::Physics::DynamicMesh::GetPhysicsLockYRotation, 
						&Core::Physics::DynamicMesh::SetPhysicsLockYRotation)
			.property("RotationLockZ", &Core::Physics::DynamicMesh::GetPhysicsLockZRotation, 
						&Core::Physics::DynamicMesh::SetPhysicsLockZRotation)
			;

		}


		DynamicMesh::DynamicMesh(Collider* collider) :
			Core::Datastructure::IPhysics(collider),
			m_dynamicMesh{ nullptr }
		{
		}

		void DynamicMesh::OnStart()
		{
			GetParent()->SetAnEventTransformChange(std::bind(&DynamicMesh::SetPhysicsTransformParent, this));
			IPhysics::OnStart();
			IUpdatable::OnStart();

		}

		void DynamicMesh::StartCopy(IComponent*& copyTo) const
		{
			copyTo = new DynamicMesh();
			OnCopy(copyTo);

		}

		void DynamicMesh::OnCopy(IComponent* copyTo) const
		{
			ComponentBase::OnCopy(copyTo);
			IPhysics::OnCopy(copyTo);
			IUpdatable::OnCopy(copyTo);

			DynamicMesh* phy = dynamic_cast<DynamicMesh*>(copyTo);

			phy->m_dynamicMesh = m_dynamicMesh;

		}

		void DynamicMesh::DestroyDynamicMesh()
		{
			GetScene()->GetEngine()->GetPhysicsScene()->RemoveActorFromPhysicsScene(m_dynamicMesh);
			m_dynamicMesh->release();
		}

		void DynamicMesh::OnDestroy()
		{
			DestroyDynamicMesh();
			ComponentBase::OnDestroy();
			IPhysics::OnDestroy();
			IUpdatable::OnDestroy();
			GetParent()->RemoveEventTransformChange();
		}

		void DynamicMesh::OnReset()
		{
			// TODO
			//Maybe To improve
			DestroyDynamicMesh();
			ComponentBase::OnReset();
			IPhysics::OnReset();
			IUpdatable::OnReset();
		}

		void DynamicMesh::CreateActor(physx::PxPhysics* physics, physx::PxScene* scene)
		{
			Maths::Vec3 vec = GetParent()->GetGlobalPos();
			physx::PxVec3 position = physx::PxVec3{ vec.x, vec.y, vec.z };
			Maths::Quat quat = GetParent()->GetGlobalRot();
			physx::PxQuat rotation = physx::PxQuat{ quat.x, quat.y, quat.z, quat.w };
			m_dynamicMesh = physics->createRigidDynamic(physx::PxTransform(position, rotation));
			m_dynamicMesh->attachShape(*m_collider->GetShape());
			
			m_dynamicMesh->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, true);
			
			// usefull if we don't want tu do the update every time if the rigid doesn't move
			//m_dynamicMesh->setActorFlag(physx::PxActorFlag::eSEND_SLEEP_NOTIFIES);
			
			//setupFiltering(m_dynamicMesh, EFilterCollision::T1, EFilterCollision::T2, EFilterCollision::T3);

			m_dynamicMesh->userData = static_cast<void*>(dynamic_cast<Core::Datastructure::IPhysics*>(this));

			scene->addActor(*m_dynamicMesh);
			PhysicsLockRotation(true, false, true);
		}

		void DynamicMesh::SetPhysicsTransformParent()
		{
			if (m_dynamicMesh == nullptr || m_MeshChangeGlobalPos)
			{
				m_MeshChangeGlobalPos = false;
				return;
			}

			Maths::Vec3 vec = GetParent()->GetGlobalPos();
			physx::PxVec3 position = physx::PxVec3{ vec.x, vec.y, vec.z };
			Maths::Quat quat = GetParent()->GetGlobalRot();
			physx::PxQuat rotation = physx::PxQuat{ quat.x, quat.y, quat.z, quat.w };

			m_dynamicMesh->setGlobalPose(physx::PxTransform(position, rotation));

			ClearForces();
		}

		void DynamicMesh::OnUpdate(float deltaTime)
		{
			
			physx::PxVec3 pos = m_dynamicMesh->getGlobalPose().p;
			physx::PxQuat rot = m_dynamicMesh->getGlobalPose().q;

			m_MeshChangeGlobalPos = true;
			GetParent()->SetGlobalPos({ pos.x, pos.y, pos.z });
			m_MeshChangeGlobalPos = true;
			GetParent()->SetGlobalRot({ rot.w, rot.x, rot.y, rot.z });
			float y = m_dynamicMesh->getLinearVelocity().y;

		}

		void DynamicMesh::SetLinearVelocity(Core::Maths::Vec3 newVelocity)
		{
			m_dynamicMesh->setLinearVelocity({ newVelocity.x, newVelocity.y, newVelocity.z });
		}

		Core::Maths::Vec3 DynamicMesh::GetVelocity()
		{
			physx::PxVec3 vec{ m_dynamicMesh->getLinearVelocity() };
			return { vec.x, vec.y, vec.z };
		}

		void DynamicMesh::AddVelocity(const Core::Maths::Vec3 vector)
		{
			m_dynamicMesh->addForce({ vector.x,vector.y,vector.z }, physx::PxForceMode::eVELOCITY_CHANGE);
		}

		void DynamicMesh::SetMass(const float mass)
		{
			m_dynamicMesh->setMass(static_cast<physx::PxReal>(mass));
		}

		float DynamicMesh::GetMass()
		{
			return m_dynamicMesh->getMass();
		}

		void DynamicMesh::ClearForces()
		{
			m_dynamicMesh->clearForce();
			m_dynamicMesh->clearTorque();
		}

		void DynamicMesh::PhysicsLockRotation(bool Axisx, bool Axisy, bool Axisz)
		{
			SetPhysicsLockXRotation(Axisx);
			SetPhysicsLockYRotation(Axisy);
			SetPhysicsLockZRotation(Axisz);
		}


		void DynamicMesh::SetPhysicsLockXRotation(bool Axisx)
		{
			m_dynamicMesh->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, Axisx);
		}

		bool DynamicMesh::GetPhysicsLockXRotation()
		{
			return m_dynamicMesh->getRigidDynamicLockFlags().isSet(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X);
		}

		void DynamicMesh::SetPhysicsLockYRotation(bool Axisy)
		{
			m_dynamicMesh->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, Axisy);
		}

		bool DynamicMesh::GetPhysicsLockYRotation()
		{
			return m_dynamicMesh->getRigidDynamicLockFlags().isSet(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y);
		}

		void DynamicMesh::SetPhysicsLockZRotation(bool Axisz)
		{
			m_dynamicMesh->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, Axisz);
		}

		bool DynamicMesh::GetPhysicsLockZRotation()
		{
			return m_dynamicMesh->getRigidDynamicLockFlags().isSet(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
		}
	}
}