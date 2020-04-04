#pragma once
#include "CoreMinimal.h"
#include "PxSimulationEventCallback.h"


namespace Core::Physics
{
	class PhysicsSceneSimulationEventCallback : public physx::PxSimulationEventCallback
	{
	public:
		virtual void 	onConstraintBreak(physx::PxConstraintInfo * constraints, physx::PxU32 count) override
		{
		}
		virtual void 	onWake(physx::PxActor * *actors, physx::PxU32 count) override
		{
		}
		virtual void 	onSleep(physx::PxActor * *actors, physx::PxU32 count) override
		{
		}
		virtual void 	onContact(const physx::PxContactPairHeader & pairHeader, const physx::PxContactPair * pairs, physx::PxU32 nbPairs) override;
		virtual void 	onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
		virtual void 	onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override
		{
		}
	};

	static physx::PxFilterFlags filterShader(
		physx::PxFilterObjectAttributes attributes0,
		physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1,
		physx::PxFilterData filterData1,
		physx::PxPairFlags& pairFlags,
		const void* constantBlock,
		physx::PxU32 constantBlockSize)
	{
		std::cout << "hit" << std::endl;
		if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
		{
			//std::cout << "just trigger" << std::endl;
			pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
			return physx::PxFilterFlag::eDEFAULT;
		}

		// generate contacts for all that were not filtered above

		// trigger the contact callback for pairs (A,B) where
		// the filtermask of A contains the ID of B and vice versa.

		//pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
		pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
		pairFlags |= physx::PxPairFlag::eDETECT_DISCRETE_CONTACT;
		pairFlags |= physx::PxPairFlag::eDETECT_CCD_CONTACT;
		pairFlags |= physx::PxPairFlag::eSOLVE_CONTACT;

		return physx::PxFilterFlag::eDEFAULT;
	}
}

