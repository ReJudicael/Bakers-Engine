#pragma once
#include "IComponent.hpp"
#include "Collider.h"
#include "CoreMinimal.h"
#include "IPhysicsEvent.h"

namespace physx
{
	class PxPhysics;
	class PxScene;
}
namespace Core
{
	namespace Datastructure
	{

		BAKERS_API_CLASS IPhysics : public virtual IComponent
		{
		protected:
			Physics::Collider*		m_collider;

		public:

			IPhysicsEvent m_event;

			inline virtual Physics::Collider* GetCollider()
			{
				return m_collider;
			}

			IPhysics();
			IPhysics(Physics::Collider* collider);
			virtual void OnDestroy() override;
			virtual void OnStart() override;

			virtual void CreateActor(physx::PxPhysics* physics, physx::PxScene* scene) {};

			REGISTER_CLASS(IComponent)
		};
	}
}

