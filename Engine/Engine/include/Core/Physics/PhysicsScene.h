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
	//class PxRigidStatic;
	struct PxRaycastHit;
	struct PxRigidActor;
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
		class IComponent;
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

			/**
			 * Init the value of the HitResult
			 * @param raycastHit: the raycastHit of a raycast
			 */
			void initHitResult(const physx::PxRaycastHit raycastHit);
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

			physx::PxRigidActor* CreateEditorPhysicsActor(void* useDataPtr,
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

			/*
			 * Do a raycast in the physics scene and return one hit result the nearest
			 * @param OriginPos: the position of the begin of the raycast
			 * @param Direction: the direction of the raycast
			 * @param result: the hit result of the raycast
			 * @param Distance: the distance of the raycast
			 * by default = FLT_MAX
			 * @return true if the raycast hit something
			 */
			bool Raycast(const Core::Maths::Vec3& OriginPos, const Core::Maths::Vec3& Direction, HitResult& result, const float Distance = FLT_MAX);

			/*
			 * Do a raycast in the physics scene and return all the hit result find
			 * @param OriginPos: the position of the begin of the raycast
			 * @param Direction: the direction of the raycast
			 * @param results: all the hit result of the raycast
			 * @param Distance: the distance of the raycast
			 * by default = FLT_MAX
			 * @return true if the raycast hit something
			 */
			bool Raycast(const Core::Maths::Vec3& OriginPos, const Core::Maths::Vec3& Direction, std::vector<HitResult>& results, const float Distance = FLT_MAX);


			void UpdatePoseOfActor(physx::PxRigidActor* actor);

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

			/*
			 * Remove an Actor from the physics Scene
			 * @param actor: the actor wich will be destroyed
			 */
			void RemoveActorFromPhysicsScene(physx::PxRigidActor* actor);

			/*
			 * Default Destructor, call ReleasePhysXSDK()
			 */
			~PhysicsScene();
		};
	}
}

