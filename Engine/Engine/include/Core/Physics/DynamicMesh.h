#pragma once

#include "CoreMinimal.h"
#include "ComponentBase.h"
#include "ICollidable.h"
#include "IPhysics.h"
#include "IUpdatable.hpp"

namespace physx
{
	class PxRigidDynamic;
	class PxPhysics;
	class PxScene;
}

namespace Core
{
	namespace Physics
	{

		BAKERS_API_CLASS DynamicMesh : public Core::Datastructure::ComponentBase, public virtual Core::Datastructure::IPhysics, public virtual Core::Datastructure::IUpdatable
		{
		private:
			physx::PxRigidDynamic* m_dynamicMesh;
		protected:
			virtual void OnDestroy() override;
		public:
			DynamicMesh() = default;
			DynamicMesh(Collider* collider);
			virtual void OnStart() override;
			virtual void OnUpdate(float deltaTime) override;
			virtual void CreateActor(physx::PxPhysics* physics, physx::PxScene* scene) override;


			REGISTER_CLASS(Core::Datastructure::ComponentBase, Core::Datastructure::IPhysics, Core::Datastructure::IUpdatable)
		};
	}
}

