#include "PxScene.h"
#include "PxRigidStatic.h"
#include "PxPhysics.h"

#include "StaticMesh.h"
#include "RootObject.hpp"
#include "PhysicsScene.h"
#include "Collider.h"
#include "EngineCore.h"

namespace Core
{
	namespace Physics
	{
		StaticMesh::StaticMesh(Collider* collider):
			IPhysics(collider)
		{
		}

		void StaticMesh::OnStart()
		{
			IPhysics::OnStart();
			IUpdatable::OnStart();

		}

		void StaticMesh::OnDestroy()
		{
			ComponentBase::OnDestroy();
			IPhysics::OnDestroy();
			IUpdatable::OnDestroy();
		}

		void StaticMesh::CreateActor(physx::PxPhysics* physics, physx::PxScene* scene)
		{
			Maths::Vec3 vec = GetParent()->GetGlobalPos();
			physx::PxVec3 position = physx::PxVec3{ vec.x, vec.y, vec.z };
			Maths::Quat quat = GetParent()->GetGlobalRot();
			physx::PxQuat rotation = physx::PxQuat{ quat.x, quat.y, quat.z, quat.w };

			m_staticMesh = physics->createRigidStatic(physx::PxTransform(position, rotation));
			//shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
			//shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
			m_staticMesh->attachShape(*m_collider->GetShape());
			//setupFiltering(myActor3, EFilterCollision::JUDICAEL, EFilterCollision::VALENTIN, EFilterCollision::LAVIE | EFilterCollision::NATHAN);
			//setupFiltering(myActor, EFilterCollision::JUDICAEL, EFilterCollision::NATHAN, EFilterCollision::LAVIE | EFilterCollision::VALENTIN);

			m_staticMesh->userData = static_cast<void*>(dynamic_cast<Core::Datastructure::IPhysics*>(this));;

			scene->addActor(*m_staticMesh);
			m_collider->GetShape()->release();
		}
	}
}


