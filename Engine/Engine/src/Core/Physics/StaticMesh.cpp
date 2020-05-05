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
		RTTR_PLUGIN_REGISTRATION
		{
			registration::class_<Core::Physics::StaticMesh>("Static Mesh")
			.constructor()
			.constructor<Collider*>()
			;
		}
		StaticMesh::StaticMesh(Collider* collider):
			IPhysics(collider)
		{
		}

		bool StaticMesh::OnStart()
		{
			GetParent()->SetAnEventTransformChange(std::bind(&StaticMesh::SetPhysicsTransformParent, this));
			return IPhysics::OnStart() && IUpdatable::OnStart();

		}

		void StaticMesh::StartCopy(IComponent*& copyTo) const
		{
			copyTo = new StaticMesh();
			OnCopy(copyTo);
		}

		void StaticMesh::OnCopy(IComponent* copyTo) const
		{
			ComponentBase::OnCopy(copyTo);
			IPhysics::OnCopy(copyTo);
			IUpdatable::OnCopy(copyTo);

			StaticMesh* phy = dynamic_cast<StaticMesh*>(copyTo);

			phy->m_staticMesh = m_staticMesh;

		}

		void StaticMesh::DestroyStaticMesh()
		{
			GetRoot()->GetEngine()->GetPhysicsScene()->RemoveActorFromPhysicsScene(m_staticMesh);
			m_staticMesh->release();
			m_staticMesh = nullptr;
		}

		void StaticMesh::OnDestroy()
		{
			ComponentBase::OnDestroy();
			IPhysics::OnDestroy();
			IUpdatable::OnDestroy();
			if (IsStarted())
			{
				DestroyStaticMesh();
				GetParent()->RemoveEventTransformChange();
			}
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
			m_staticMesh->attachShape(*m_collider->GetShape());
			m_staticMesh->userData = static_cast<void*>(dynamic_cast<Core::Datastructure::IComponent*>(dynamic_cast<Core::Datastructure::IPhysics*>(this)));
			

			scene->addActor(*m_staticMesh);
			//m_collider->GetShape()->release();
		}

		void StaticMesh::SetPhysicsTransformParent()
		{
			if (m_staticMesh == nullptr)
				return;

			Maths::Vec3 vec = GetParent()->GetGlobalPos();
			physx::PxVec3 position = physx::PxVec3{ vec.x, vec.y, vec.z };
			Maths::Quat quat = GetParent()->GetGlobalRot();
			physx::PxQuat rotation = physx::PxQuat{ quat.x, quat.y, quat.z, quat.w };

			m_staticMesh->setGlobalPose(physx::PxTransform(position, rotation));
		}
	}
}


