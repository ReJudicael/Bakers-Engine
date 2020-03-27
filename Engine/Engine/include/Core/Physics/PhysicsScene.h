#pragma once


namespace physx
{
	class PxFoundation;
	class PxPvd;
	class PxPvdTransport;
	class PxPhysics;
	class PxCooking;
	class PxScene;
	/*class PxDefaultErrorCallback;
	class PxDefaultAllocator;*/
}

#define NEED_PVD

namespace Core::Physics
{
	class PhysicsScene
	{
	private:
		physx::PxFoundation* m_pxFoundation;
		physx::PxPvd* m_pxPvd;
		physx::PxPvdTransport* m_pxTransport;
		physx::PxPhysics* m_pxPhysics;
		physx::PxCooking* m_pxCooking;
		physx::PxScene* m_pxScene;
		//physx::PxDefaultErrorCallback	m_pxDefaultErrorCallback;
		//physx::PxDefaultAllocator		m_pxDefaultAllocatorCallback;
		bool							m_IsSimulating;
		float							m_accumulator{ 0.f };
		float							m_stepSimulation{ 1.f/60.f };
	public:

		bool InitPhysX();
		void Simulate(const float deltaTime);
	};
}

