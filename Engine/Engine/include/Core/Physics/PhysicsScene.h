#pragma once

#include "PxDefaultAllocator.h"
#include "PxDefaultErrorCallback.h"
#include "PxSimulationEventCallback.h"
#include "IPhysics.h"
#include "PxSimpleTypes.h"

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
	struct PxOverlapHit;
	class PxGeometry;
	class PxRigidActor;

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


		enum EFilterRaycast
		{
			GROUPE1 = (1<<0),
			GROUPE2 = (1<<1),
			GROUPE3 = (1<<2),
			GROUPE4 = (1<<3),
		};

		
		//const EFilterRaycast& GetFilterRaycast(const physx::PxFilterData& filter);

		struct HitResultQuery
		{
			Core::Datastructure::Object*		objectHit;
			Core::Datastructure::IPhysics*		physicsMeshHit;
			Core::Maths::Vec3					hitPoint;
			float								distance;

			/**
			 * Init the value of the HitResult
			 * @param raycastHit: the raycastHit of a raycast
			 */
			void initHitResult(const physx::PxRaycastHit raycastHit);

			void initHitResult(const physx::PxOverlapHit overlapHit);
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
			bool Raycast(const Core::Maths::Vec3& OriginPos, const Core::Maths::Vec3& Direction, HitResultQuery& result, const float Distance = FLT_MAX);

			/*
			 * Do a raycast in the physics scene and return all the hit result find
			 * @param OriginPos: the position of the begin of the raycast
			 * @param Direction: the direction of the raycast
			 * @param results: all the hit result of the raycast
			 * @param Distance: the distance of the raycast
			 * by default = FLT_MAX
			 * @return true if the raycast hit something
			 */
			bool Raycast(const Core::Maths::Vec3& OriginPos, const Core::Maths::Vec3& Direction, std::vector<HitResultQuery>& results, const float Distance = FLT_MAX);

			/*
			 * Do a raycast in the physics scene with filter and return one hit result the nearest
			 * @param OriginPos: the position of the begin of the raycast
			 * @param Direction: the direction of the raycast
			 * @param result: the hit result of the raycast
			 * @param Distance: the distance of the raycast
			 * @param filterRaycast: the filters of the raycast, 
			 * the objects who have this filters are not going to be test
			 * by default = FLT_MAX
			 * @return true if the raycast hit something
			 */
			bool Raycast(const Core::Maths::Vec3& OriginPos, const Core::Maths::Vec3& Direction, HitResultQuery& result, physx::PxU32 filterRaycast, const float Distance = FLT_MAX);

			/*
			 * Do a raycast in the physics scene with filter and return all the hit result find
			 * @param OriginPos: the position of the begin of the raycast
			 * @param Direction: the direction of the raycast
			 * @param results: all the hit result of the raycast
			 * @param Distance: the distance of the raycast
			 * @param filterRaycast: the filters of the raycast,
			 * the objects who have this filters are not going to be test
			 * by default = FLT_MAX
			 * @return true if the raycast hit something
			 */
			bool Raycast(const Core::Maths::Vec3& OriginPos, const Core::Maths::Vec3& Direction, std::vector<HitResultQuery>& results, physx::PxU32 filterRaycast, const float Distance = FLT_MAX);


			bool CheckOverlap(const physx::PxGeometry& overlapGeometry, const Core::Datastructure::Transform& position, HitResultQuery& overlapResult);

			bool CheckOverlap(const physx::PxGeometry& overlapGeometry, const Core::Datastructure::Transform& position, std::vector<HitResultQuery>& overlapResults);
			/*
			 * Update the transform of a physics actor, call as an event, use mostly with 
			 * the editor physics
			 * @param actor: the actor we want to change
			 */
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

