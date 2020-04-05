#pragma once

#include "PxDefaultAllocator.h"
#include "PxDefaultErrorCallback.h"
#include "PxSimulationEventCallback.h"
#include "IPhysics.h"

namespace physx
{
	class PxFoundation;
	class PxPvd;
	class PxPvdTransport;
	class PxPhysics;
	class PxCooking;
	class PxScene;
	class PxShape;
	class PxErrorCallback;
}

#define NEED_PVD

namespace Core::Physics
{
	class StaticMesh;
	class Collider;
	/**
	 * Contains all the PhysX properties for create a PhysXScene
	 */
	class PhysicsScene
	{
	private:
		physx::PxFoundation*			m_pxFoundation;
		physx::PxPvd*					m_pxPvd;
		physx::PxPvdTransport*			m_pxTransport;
		physx::PxPhysics*				m_pxPhysics;
		physx::PxCooking*				m_pxCooking;
		physx::PxScene*					m_pxScene;

		physx::PxDefaultErrorCallback	m_pxDefaultErrorCallback;
		physx::PxDefaultAllocator		m_pxDefaultAllocatorCallback;

		bool							m_IsSimulating;
		float							m_accumulator{ 0.f };
		float							m_stepSimulation{ 1.f/60.f };
	public:

		/**
		 * Init the instance of PhysX for create the PhysXScene
		 */
		bool InitPhysX();

		/**
		 * Attach and create a PhysX actor
		 * @param physics: the physics mesh that we want to attach and create
		 * for this PhysicsScene
		 */
		void AttachActor(Core::Datastructure::IPhysics* physics);

		/**
		 * create a PhysX shape
		 * @param collider: the colliderthat we want to create
		 * for this PhysicsScene
		 */
		void CreatePhysicsShape(Collider& collider);

		/**
		 * simulate the physics of the scene
		 * @param deltaTime: the deltaTime give by the update
		 */
		void BeginSimulate(const float deltaTime);

		/**
		 * Create the PhysX scene
		 */
		void CreateScene();

		/**
		 * Fetch the result of the simulation
		 */
		void EndSimulate();

		/*
		 * Release all PhysXSDK
		 */
		void ReleasePhysXSDK();

		void RemoveActorFromPhysicsScene(physx::PxRigidActor* actor);

		/*
		 * Default Destructor, call ReleasePhysXSDK()
		 */
		~PhysicsScene();
	};
}

