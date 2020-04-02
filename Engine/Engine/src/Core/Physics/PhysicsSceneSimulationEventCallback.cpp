#include <iostream>

#include "PhysicsSceneSimulationEventCallback.h"
#include "PxRigidActor.h"
#include "IPhysics.h"

namespace Core::Physics
{
	void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) 
	{
		/*for (physx::PxU32 i; i < nbPairs; i++)
		{
			pairs[i].
		}*/
	}
	void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
	{
		//Core::Datastructure::IPhysicsEvent* event = static_cast<Core::Datastructure::IPhysicsEvent*>(pairs->triggerActor->userData);
		Core::Datastructure::IPhysics* triggered = static_cast<Core::Datastructure::IPhysics*>(pairs->triggerActor->userData);
		Core::Datastructure::IPhysics* triggerWith = static_cast<Core::Datastructure::IPhysics*>(pairs->otherActor->userData);
		if (pairs->status == physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			triggered->m_event.OnTriggerEnterEvent.Invoke(triggerWith);
		}
		else if (pairs->status == physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			triggered->m_event.OnTriggerExitEvent.Invoke(triggerWith);
		}
	}
}