#include "DynamicMesh.h"
#include "PxScene.h"
#include "PxRigidStatic.h"
#include "PxRigidDynamic.h"
#include "PxPhysics.h"

#include "RootObject.hpp"
#include "PhysicsScene.h"
#include "EngineCore.h"

namespace Core
{
	namespace Physics
	{
		DynamicMesh::DynamicMesh(Collider* collider) :
			Core::Datastructure::IPhysics(collider)
		{
		}

		void DynamicMesh::OnStart()
		{
			IPhysics::OnStart();
			IUpdatable::OnStart();

		}
		void DynamicMesh::OnDestroy()
		{
			ComponentBase::OnDestroy();
			IPhysics::OnDestroy();
			IUpdatable::OnDestroy();
		}

		void DynamicMesh::CreateActor(physx::PxPhysics* physics, physx::PxScene* scene)
		{
			Maths::Vec3 vec = GetParent()->GetPos();
			physx::PxVec3 position = physx::PxVec3{ vec.x, vec.y, vec.z };
			Maths::Quat quat = GetParent()->GetRot();
			physx::PxQuat rotation = physx::PxQuat{ quat.x, quat.y, quat.z, quat.w };
			m_dynamicMesh = physics->createRigidDynamic(physx::PxTransform(position, rotation));
			//shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
			//shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
			m_dynamicMesh->attachShape(*m_collider->GetShape());
			
			m_dynamicMesh->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, true);
			
			// usefull if we don't want tu do the update every time if the rigid doesn't move
			//m_dynamicMesh->setActorFlag(physx::PxActorFlag::eSEND_SLEEP_NOTIFIES);
			
			//setupFiltering(m_dynamicMesh, EFilterCollision::T1, EFilterCollision::T2, EFilterCollision::T3);

			m_dynamicMesh->userData = static_cast<void*>(dynamic_cast<Core::Datastructure::IPhysics*>(this));

			scene->addActor(*m_dynamicMesh);
			m_collider->GetShape()->release();
		}

		void DynamicMesh::OnUpdate(float deltaTime)
		{
			
			physx::PxVec3 pos = m_dynamicMesh->getGlobalPose().p;
			physx::PxQuat rot = m_dynamicMesh->getGlobalPose().q;

			GetParent()->SetPos({ pos.x, pos.y, pos.z });
			GetParent()->SetRot({ rot.w, rot.x, rot.y, rot.z });
		}
	}
}