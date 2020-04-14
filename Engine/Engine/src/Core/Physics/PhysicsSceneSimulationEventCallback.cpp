#include <iostream>

#include "PhysicsSceneSimulationEventCallback.h"
#include "PxRigidActor.h"
#include "IPhysics.h"

namespace Core::Physics
{
	void PhysicsSceneSimulationEventCallback::onContact(const physx::PxContactPairHeader& pairHeader, 
														const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
	{	
		Core::Datastructure::IPhysics* contact1 = dynamic_cast<Core::Datastructure::IPhysics*>(static_cast<Core::Datastructure::IComponent*>(pairHeader.actors[0]->userData));
		Core::Datastructure::IPhysics* contact2 = dynamic_cast<Core::Datastructure::IPhysics*>(static_cast<Core::Datastructure::IComponent*>(pairHeader.actors[1]->userData));
		
		contact1->OnContactEvent(contact2);
		contact2->OnContactEvent(contact1);
	}
	void PhysicsSceneSimulationEventCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
	{
		Core::Datastructure::IPhysics* triggered = dynamic_cast<Core::Datastructure::IPhysics*>(static_cast<Core::Datastructure::IComponent*>(pairs->triggerActor->userData));
		Core::Datastructure::IPhysics* triggerWith = dynamic_cast<Core::Datastructure::IPhysics*>(static_cast<Core::Datastructure::IComponent*>(pairs->otherActor->userData));
		if (pairs->status == physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			triggered->OnTriggerEnterEvent.Invoke(triggerWith);
		}
		else if (pairs->status == physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			triggered->OnTriggerExitEvent.Invoke(triggerWith);
		}
	}

	physx::PxFilterFlags PhysicsSceneSimulationEventCallback::filterShader(
		physx::PxFilterObjectAttributes attributes0,
		physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1,
		physx::PxFilterData filterData1,
		physx::PxPairFlags& pairFlags,
		const void* constantBlock,
		physx::PxU32 constantBlockSize)
	{
		if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
			return physx::PxFilterFlag::eDEFAULT;
		}

		// generate contacts for all that were not filtered above

		//pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
		pairFlags = physx::PxPairFlag::eSOLVE_CONTACT;
		pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
		pairFlags |= physx::PxPairFlag::eDETECT_DISCRETE_CONTACT;
		pairFlags |= physx::PxPairFlag::eDETECT_CCD_CONTACT;

		return physx::PxFilterFlag::eDEFAULT;
	}
}