#pragma once
#include "CoreMinimal.h"
#include "PxSimulationEventCallback.h"


namespace Core::Physics
{
	/*
	 * A class who inherit from PxSimulationEventCallback
	 * for create our on callback event
	 */
	class PhysicsSceneSimulationEventCallback : public physx::PxSimulationEventCallback
	{
	public:

		/**
		 * Function inheritated from PxSimulationEventCallback
		 * @param constraints: The constraints which have been broken.
		 * @param count: The number of constraints
		 */
		virtual void 	onConstraintBreak(physx::PxConstraintInfo * constraints, physx::PxU32 count) override
		{
		}

		/**
		 * Function inheritated from PxSimulationEventCallback
		 * @param actors: The actors which just woke up.
		 * @param  count: The number of actors
		 */
		virtual void 	onWake(physx::PxActor * *actors, physx::PxU32 count) override
		{
		}

		/**
		 * Function inheritated from PxSimulationEventCallback
		 * @param actors: The actors which have just been put to sleep.
		 * @param count: The number of actors
		 */
		virtual void 	onSleep(physx::PxActor * *actors, physx::PxU32 count) override
		{
		}

		/**
		 * Function inheritated from PxSimulationEventCallback,
		 * Override for call the EventSystem OnContactEvent
		 * @param pairHeader: Information on the two actors whose shapes triggered a contact report.
		 * @param pairs: The contact pairs of two actors for which contact reports have been requested. See #PxContactPair.
		 * @param nbPairs: The number of provided contact pairs.
		 */
		virtual void 	onContact(const physx::PxContactPairHeader & pairHeader, 
									const physx::PxContactPair * pairs, 
									physx::PxU32 nbPairs) override;

		/**
		 * Function inheritated from PxSimulationEventCallback,
		 * Override for call the EventSystems OnTriggerEnterEvent and OnTriggerExitEvent
		 * @param pairs: The trigger pair events.
		 * @param count: The number of trigger pair events.
		 */
		virtual void 	onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;

		/**
		 * Function inheritated from PxSimulationEventCallback,
		 * @param bodyBuffer: The rigid bodies that moved and requested early pose reporting.
		 * @param poseBuffer: The integrated rigid body poses of the bodies listed in bodyBuffer.
	     * @param count: The number of entries in the provided buffers.
		 */
		virtual void 	onAdvance(const physx::PxRigidBody* const* bodyBuffer, 
									const physx::PxTransform* poseBuffer, 
									const physx::PxU32 count) override
		{
		}


		static physx::PxFilterFlags filterShader(
			physx::PxFilterObjectAttributes attributes0,
			physx::PxFilterData filterData0,
			physx::PxFilterObjectAttributes attributes1,
			physx::PxFilterData filterData1,
			physx::PxPairFlags& pairFlags,
			const void* constantBlock,
			physx::PxU32 constantBlockSize);
		
	};
}

