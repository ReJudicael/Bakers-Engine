#pragma once

#include "PxDefaultAllocator.h"
#include "PxDefaultErrorCallback.h"
#include "PxSimulationEventCallback.h"
#include "PhysicsSceneSimulationEventCallback.h"
#include "PxSimpleTypes.h"
#include "Vec3.hpp"
#include "Object.hpp"

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

		constexpr unsigned int MAX_QUERY_HIT_BUFFER = 20;
		enum class EFilterRaycast : int
		{
			GROUPE1 = (1<<0),
			GROUPE2 = (1<<1),
			GROUPE3 = (1<<2),
			GROUPE4 = (1<<3),
		};

		enum class EGeometry : int
		{
			BOX = 0,
			CAPSULE,
			SPHERE,
		};

		struct HitResultQuery
		{
			Core::Datastructure::Object*		objectHit;
			Core::Physics::Collider*		physicsMeshHit;
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
		 * Contains all the PhysX properties for create a PhysXScene and simulate the physics
		 */
		BAKERS_API_CLASS PhysicsScene
		{
		private:
			physx::PxFoundation*					m_pxFoundation{ nullptr };
			physx::PxPvd*							m_pxPvd{ nullptr };
			physx::PxPvdTransport*					m_pxTransport{ nullptr };
			physx::PxPhysics*						m_pxPhysics{ nullptr };
			physx::PxCooking*						m_pxCooking{ nullptr };
			physx::PxScene*							m_pxScene{ nullptr };
			PhysicsSceneSimulationEventCallback* m_eventCallBack{ nullptr };

			physx::PxDefaultErrorCallback	m_pxDefaultErrorCallback;
			physx::PxDefaultAllocator		m_pxDefaultAllocatorCallback;

			bool							m_IsSimulating{ false };
			float							m_accumulator{ 0.f };
			float							m_stepSimulation{ 1.f / 60.f };
			
		protected:
			/*
			 * Release all the scene create by physX
			 */
			void ReleaseAllScene();

		public:
			/**
			 * Default constructor
			 */
			PhysicsScene() = default;


			/**
			 * Init the instance of PhysX for create the PhysXScene
			 */
			bool InitPhysX();

			/**
			 * Attach and create a PhysX actor
			 * @param physics: the physics mesh that we want to attach and create
			 * for this PhysicsScene
			 */
			//void AttachActor(Core::Datastructure::IPhysics* physics);

			/*
			 * Create a PhysX PxRigidStatic
			 * @param actor: the PxRigidActor who is gonna be create as a PxRigidStatic
			 * @param shape: the shape use for create the actor
			 * @param transform: the object transform
			 * @return the pointer of the PxRigidStatic created
			 */
			physx::PxRigidStatic* CreateRigidStatic(physx::PxRigidActor*& actor, physx::PxShape* shape, Core::Datastructure::Transform transform);
			/*
			 * Create a PhysX PxRigidDynamic
			 * @param actor: the PxRigidActor who is gonna be create as a PxRigidDynamic
			 * @param shape: the shape use for create the actor
			 * @param transform: the object transform
			 * @return the pointer of the PxRigidDynamic created
			 */
			physx::PxRigidDynamic* CreateRigidDynamic(physx::PxRigidActor*& actor, physx::PxShape* shape, Core::Datastructure::Transform transform);


			/*
			 * Create a PxRigidStatic as a bax with the AABB value of the mesh model
			 * @param useDataPtr: the userData for the userData of the PxRigidStatic
			 * @param transform: the object transform of the mesh
			 * @param model: the mesh model
			 * @return the actor wich is create
			 */
			physx::PxRigidActor* CreateEditorPhysicsActor(void* useDataPtr,
															const Core::Datastructure::Transform& transform,
														std::shared_ptr<Resources::Model> model, physx::PxScene*& scene);
			/*
			 * Create a the box shape of the PxRigidStatic for the Editor
			 * @param transform: the object transform of the mesh
			 * @param model: the mesh model
			 * @return the shape wich is create
			 */
			physx::PxShape* CreateEditorBoxShape(const Core::Datastructure::Transform& transform, 
													std::shared_ptr<Resources::Model> model);

			/*
			 * Create a the box shape of the PxRigidStatic for the Editor
			 * @param scale: the scale of the object
			 * @param min: the min of the model AABB
			 * @param max: the max of the model AABB
			 * @return the extent calculate
			 */
			Core::Maths::Vec3 InitExtentOfEditorBoxShape(const Core::Maths::Vec3& scale,
														const Core::Maths::Vec3& min, const Core::Maths::Vec3& max);
			/*
			 * Destroy a PxRigidActor and his shape
			 * @param actor: the physic actor we want to destroy
			 */
			void DestroyEditorPhysicActor(physx::PxRigidActor* actor, physx::PxScene*& scene);

			/**
			 * Create the PhysX scene
			 */
			void CreateScene();

			/*
			 * Create a PxScene use has a query scene
			 * @param scene: the scene create has a query
			 */
			void CreateQueryScene(physx::PxScene*& scene);

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
			 * @param result: the nearest hit result of the raycast
			 * @param Distance: the distance of the raycast
			 * by default = FLT_MAX
			 * @return true if the raycast hit something
			 */
			bool Raycast(const Core::Maths::Vec3& OriginPos, 
							const Core::Maths::Vec3& Direction, 
							HitResultQuery& result, const float Distance = FLT_MAX);

			/*
			 * Do a raycast in the physics scene and return all the hit result find
			 * @param OriginPos: the position of the begin of the raycast
			 * @param Direction: the direction of the raycast
			 * @param results: all the hit result of the raycast
			 * @param Distance: the distance of the raycast
			 * by default = FLT_MAX
			 * @return true if the raycast hit something
			 */
			bool Raycast(const Core::Maths::Vec3& OriginPos, 
						const Core::Maths::Vec3& Direction, 
						std::vector<HitResultQuery>& results, const float Distance = FLT_MAX);

			/*
			 * Do a raycast in the physics scene with filter and return one hit result the nearest
			 * @param OriginPos: the position of the begin of the raycast
			 * @param Direction: the direction of the raycast
			 * @param result: the nearest hit result of the raycast
			 * @param Distance: the distance of the raycast
			 * @param filterRaycast: the filters of the raycast, 
			 * the objects who have this filters are not going to be test
			 * by default = FLT_MAX
			 * @return true if the raycast hit something
			 */
			bool Raycast(const Core::Maths::Vec3& OriginPos, 
							const Core::Maths::Vec3& Direction, 
							HitResultQuery& result, physx::PxU32 filterRaycast, const float Distance = FLT_MAX);

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
			bool Raycast(const Core::Maths::Vec3& OriginPos, 
						const Core::Maths::Vec3& Direction, 
						std::vector<HitResultQuery>& results, physx::PxU32 filterRaycast, const float Distance = FLT_MAX);

			/**
			 * Do an overlap check in the physics scene with a gemotry gived
			 * @param overlapGeometry: the geomtry of object to check for overlap
			 * @param halfextent: the half extent of the geometry choose
			 * @param transform: the global position of the geometry
			 * @param overlapResult: the nearest hit result of the overlap
			 */
			bool CheckBoxOverlap(const Core::Maths::Vec3& halfextent,
								const Core::Datastructure::Transform& transform,
								HitResultQuery& overlapResult);

			/**
			 * Do an overlap check in the physics scene with a gemotry gived
			 * @param overlapGeometry: the geomtry of object to check for overlap
			 * @param halfextent: the half extent of the geometry choose
			 * @param transform: the global position of the geometry
			 * @param overlapResult: all the hit result of the overlap
			 */
			bool CheckBoxOverlap(const Core::Maths::Vec3& halfextent,
								const Core::Datastructure::Transform& transform, 
								std::vector<HitResultQuery>& overlapResults);
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
			 * Attach an Actor to the physics scene
			 * @param actor: the actor wich will be attach to the physics scene
			 */
			void AttachActorToPhysicsScene(physx::PxRigidActor* actor);

			/*
			 * Remove an Actor from the physics Scene
			 * @param actor: the actor wich will be remove from the physics scene
			 */
			void RemoveActorFromPhysicsScene(physx::PxRigidActor* actor);


			/*
			 * Default Destructor, call ReleasePhysXSDK()
			 */
			~PhysicsScene();
		};
	}
}

