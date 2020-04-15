#include <iostream>

#include "PhysicsSceneSimulationEventCallback.h"
#include "PxRigidActor.h"
#include "IPhysics.h"
#include "PhysicsScene.h"
#include "Object.hpp"
#include "RootObject.hpp"
#include "EngineCore.h"
#include "HitResult.h"

namespace Core::Physics
{
	void PhysicsSceneSimulationEventCallback::onContact(const physx::PxContactPairHeader& pairHeader, 
														const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
	{	
		physx::PxContactPairPoint contacts[1];
		int indexPairs{ 0 };
		for (physx::PxU32 i{ 0 }; i < nbPairs; i++)
		{
			const physx::PxContactPair& cp = pairs[i];

			if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				physx::PxU32 nbContacts = pairs[i].extractContacts(contacts, 1);

				HitResult hitResult;
				Core::Maths::Vec3 impulseVector{};
				if (nbContacts > 0)
				{
					physx::PxVec3 point = contacts[0].position;
					hitResult.impactPoint = { point.x, point.y, point.z };
					physx::PxVec3 normal = contacts[0].normal;
					hitResult.impactNormal = { normal.x, normal.y, normal.z };
					hitResult.penetrationDepth = contacts[0].separation;
					hitResult.isPenetrate = (contacts[0].separation < 0 ? false : true);
					physx::PxVec3 impulse = contacts[0].impulse;
					impulseVector = { impulse.x,impulse.y, impulse.z };
				}

				Core::Datastructure::IPhysics* contact1 = dynamic_cast<Core::Datastructure::IPhysics*>(static_cast<Core::Datastructure::IComponent*>(pairHeader.actors[0]->userData));
				Core::Datastructure::IPhysics* contact2 = dynamic_cast<Core::Datastructure::IPhysics*>(static_cast<Core::Datastructure::IComponent*>(pairHeader.actors[1]->userData));

				contact1->OnContactEvent(contact2, impulseVector, hitResult);
				contact2->OnContactEvent(contact1, impulseVector, hitResult);

				break;
			}

			/*physx::PxContactStreamIterator iter(cp.contactPatches, cp.contactPoints, cp.getInternalFaceIndices(), cp.patchCount, cp.contactCount);

			const physx::PxReal* impulses = cp.contactImpulses;

			physx::PxU32 indexContact{ 0 };
			physx::PxU32 flippedContacts{ cp.flags & physx::PxContactPairFlag::eINTERNAL_CONTACTS_ARE_FLIPPED };
			physx::PxU32 hasImpulses{ cp.flags & physx::PxContactPairFlag::eINTERNAL_HAS_IMPULSES };

			while (iter.hasNextPatch())
			{
				iter.nextPatch();
				while (iter.hasNextContact())
				{
					iter.nextContact();

					physx::PxVec3 contactPoint{ iter.getContactPoint() };

					//physx::PxVec3 impulse{ hasImpulses ? impulses[indexContact] * : physx::PxVec3{0.f} };

					physx::PxU32 internalFaceIndex0 = flippedContacts ?
						iter.getFaceIndex1() : iter.getFaceIndex0();
					physx::PxU32 internalFaceIndex1 = flippedContacts ?
						iter.getFaceIndex0() : iter.getFaceIndex1();

					indexContact++;

				}
			}*/
		}

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

		pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
		pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
		pairFlags |= physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;
		pairFlags |= physx::PxPairFlag::eDETECT_CCD_CONTACT;

		return physx::PxFilterFlag::eDEFAULT;
	}
}