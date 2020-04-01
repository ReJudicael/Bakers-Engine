#pragma once
#include "IComponent.hpp"
#include "Collider.h"
#include "CoreMinimal.h"

namespace physx
{
	class PxPhysics;
	class PxScene;
}
namespace Core
{
	namespace Datastructure
	{
		class IPhysics : public virtual IComponent
		{
		protected:
			Physics::Collider		m_collider;
		public:
			IPhysics() = default;
			IPhysics(Physics::Collider collider);
			virtual void OnDestroy() override {}
			virtual void OnStart() override;

			virtual void CreateActor(physx::PxPhysics* physics, physx::PxScene* scene) {};

			REGISTER_CLASS(IComponent)
		};
	}
}

