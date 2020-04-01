#pragma once
#include "CoreMinimal.h"
#include "ComponentBase.h"
#include "ICollidable.h"
#include "IPhysics.h"
#include "IUpdatable.hpp"

namespace physx
{
	class PxRigidStatic;
	class PxPhysics;
	class PxScene;
}

namespace Core
{
	namespace Physics
	{
		BAKERS_API_CLASS StaticMesh : public Core::Datastructure::ComponentBase, public virtual Core::Datastructure::IPhysics, public virtual Core::Datastructure::IUpdatable
		{

		protected:
			physx::PxRigidStatic*	m_staticMesh;

		public:

			StaticMesh() = default;
			StaticMesh(Collider collider);

			inline virtual void SetStaticMesh(physx::PxRigidStatic* staticMesh)
			{
				m_staticMesh = staticMesh;
			}

			inline virtual physx::PxRigidStatic* GetStaticMesh()
			{
				return m_staticMesh;
			}

			virtual void OnStart() override;
			virtual void OnUpdate(float deltaTime) override {};
			virtual void CreateActor(physx::PxPhysics* physics, physx::PxScene* scene) override;

			REGISTER_CLASS(Core::Datastructure::ComponentBase, Core::Datastructure::IPhysics, Core::Datastructure::IUpdatable)
		};
	}
}

