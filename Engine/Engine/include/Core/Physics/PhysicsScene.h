#pragma once

#include "PxDefaultAllocator.h"
#include "PxDefaultErrorCallback.h"
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

		bool InitPhysX();

		void AttachActor(Core::Datastructure::IPhysics* physics);
		void CreateBoxShape(Collider& collider);
		void BeginSimulate(const float deltaTime);
		void CreateScene();
		void EndSimulate();
	};
}

