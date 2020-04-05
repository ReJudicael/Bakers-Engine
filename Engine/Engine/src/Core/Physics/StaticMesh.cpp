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
			GetParent()->SetAnEventTransformChange(std::bind(&StaticMesh::SetPhysicsTransformParent, this));
			IPhysics::OnStart();
			IUpdatable::OnStart();

		}

		void StaticMesh::DestroyStaticMesh()
		{
			GetScene()->GetEngine()->GetPhysicsScene()->RemoveActorFromPhysicsScene(m_staticMesh);
			m_staticMesh->release();
		}

		void StaticMesh::OnDestroy()
		{
			DestroyStaticMesh();
			ComponentBase::OnDestroy();
			IPhysics::OnDestroy();
			IUpdatable::OnDestroy();
			GetParent()->RemoveEventTransformChange();
		}

		void StaticMesh::OnReset()
		{
			// TODO
			//Maybe To improve
			DestroyStaticMesh();
			ComponentBase::OnReset();
			IPhysics::OnReset();
			IUpdatable::OnReset();
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
			//m_collider->GetShape()->release();
		}

		void StaticMesh::SetPhysicsTransformParent()
		{
			if (m_staticMesh == nullptr)
				return;

			Maths::Vec3 vec = GetParent()->GetPos();
			physx::PxVec3 position = physx::PxVec3{ vec.x, vec.y, vec.z };
			Maths::Quat quat = GetParent()->GetRot();
			physx::PxQuat rotation = physx::PxQuat{ quat.x, quat.y, quat.z, quat.w };

			m_staticMesh->setGlobalPose(physx::PxTransform(position, rotation));
		}
	}
}


