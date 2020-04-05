#pragma once

#include "CoreMinimal.h"
#include "ComponentBase.h"
#include "ICollidable.h"
#include "IPhysics.h"
#include "IUpdatable.hpp"

namespace physx
{
	class PxRigidDynamic;
	class PxPhysics;
	class PxScene;
}

namespace Core
{
	namespace Physics
	{
		/**
		 * Contains a collider and a link to a PxRigidDynamic from the PhysicsScene
		 */
		BAKERS_API_CLASS DynamicMesh : public Core::Datastructure::ComponentBase, public virtual Core::Datastructure::IPhysics, public virtual Core::Datastructure::IUpdatable
		{
		private:
			physx::PxRigidDynamic* m_dynamicMesh;

		protected:
			/*
			 * Release the PxRigidDynamic, 
			 * and release him from the PhysicsScene
			 */
			virtual void DestroyDynamicMesh();
			virtual void OnDestroy() override;
			virtual void OnReset() override;

		public:
			/**
			 * Default Constructor
			 */
			DynamicMesh() = default;

			/**
			 * Constructor with a Collider, for init the collider directly
			 * @param collider: the collider with wich we want to construct the physics mesh
			 */
			DynamicMesh(Collider* collider);

			/**
			 * Function inheritated from IPhysics and IUpdatable,
			 * override for call the two OnStart of IPhysics and IUpdatable
			 */
			virtual void OnStart() override;

			/**
			 * Function inheritated from IPhysics,
			 * override for create a specific physX actor a PxRigidDynamic
			 * @param physics: the PhysX physics from the PhysicsScene
			 * @param scene: the PhysX scene from the PhysicsScene
			 */
			virtual void CreateActor(physx::PxPhysics* physics, physx::PxScene* scene) override;
			/*
			 * Function inheritated from IPhysics,
			 * override for set and reset the Transform of the PxRigidDynamic
			 * and clear all the force and torque
			 */
			virtual void SetPhysicsTransformParent() override;

			/**
			 * Function inheritated from IPhysics and IUpdatable,
			 * override for assign the new position and new rotation to the Object
			 * @param deltaTime: Time since previous frame
			 */
			virtual void OnUpdate(float deltaTime) override;

			REGISTER_CLASS(Core::Datastructure::ComponentBase, Core::Datastructure::IPhysics, Core::Datastructure::IUpdatable)
		};
	}
}

