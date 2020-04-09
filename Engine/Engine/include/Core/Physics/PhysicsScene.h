#pragma once

#include "PxDefaultAllocator.h"
#include "PxDefaultErrorCallback.h"
#include "PxSimulationEventCallback.h"
#include "IPhysics.h"
#include "PxRigidStatic.h"

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
	//class PxRigidStatic;
	struct PxRaycastHit;
}

#define NEED_PVD

namespace Resources
{
	struct Model;
}

namespace Core
{
	namespace Datastructure
	{
		class Transform;
	}

	namespace Physics
	{
		class StaticMesh;
		class Collider;

		struct HitResult
		{
			Core::Datastructure::Object* objectHit;
			Core::Datastructure::IPhysics* physicsMeshHit;
			Core::Maths::Vec3					hitPoint;
			float								distance;


			void initHitResult(physx::PxRaycastHit raycastHit);
		};


		/**
		 * Contains all the PhysX properties for create a PhysXScene
		 */
		class PhysicsScene
		{
		private:
			physx::PxFoundation* m_pxFoundation;
			physx::PxPvd* m_pxPvd;
			physx::PxPvdTransport* m_pxTransport;
			physx::PxPhysics* m_pxPhysics;
			physx::PxCooking* m_pxCooking;
			physx::PxScene* m_pxScene;

			physx::PxDefaultErrorCallback	m_pxDefaultErrorCallback;
			physx::PxDefaultAllocator		m_pxDefaultAllocatorCallback;

			bool							m_IsSimulating;
			float							m_accumulator{ 0.f };
			float							m_stepSimulation{ 1.f / 60.f };
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

			physx::PxRigidStatic* CreateEditorPhysicsActor(void* useDataPtr,
															const Core::Datastructure::Transform& transform,
														std::shared_ptr<Resources::Model> model);

			/**
			 * Create the PhysX scene
			 */
			void CreateScene();

			/**
			 * create a PhysX shape
			 * @param collider: the colliderthat we want to create
			 * for this PhysicsScene
			 */
			void CreatePhysicsShape(Collider& collider);

			bool Raycast(const Core::Maths::Vec3& OriginPos, const Core::Maths::Vec3& Direction, HitResult& result, const float Distance = FLT_MAX);


			bool Raycast(const Core::Maths::Vec3& OriginPos, const Core::Maths::Vec3& Direction, std::vector<HitResult>& result, const float Distance = FLT_MAX);

			/**
			 * simulate the physics of the scene
			 * @param deltaTime: the deltaTime give by the update
			 */
			void BeginSimulate(const float deltaTime);


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
}

