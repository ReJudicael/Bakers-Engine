#pragma once

#include "CoreMinimal.h"
#include "ComponentBase.h"
#include "ICollidable.h"
#include "IPhysics.h"
#include "IUpdatable.hpp"
#include "PxRigidDynamic.h"

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

			/**
			 * Function inheritated from IPhysics and IUpdatable,
			 * override for delete the collider and release the PxRigidDynamic
			 */
			virtual void OnDestroy() override;

			/**
			 * Function inheritated from IPhysics and IUpdatable,
			 * override for reset the PxRigidDynamic
			 */
			virtual void OnReset() override;

		public:
			/**
			 * Default Constructor
			 */
			DynamicMesh() /*= default*/;

			/**
			 * Constructor with a Collider, for init the collider directly
			 * @param collider: the collider with wich we want to construct the physics mesh
			 */
			DynamicMesh(Collider* collider);

			/**
			 * Function inheritated from IPhysics and IUpdatable,
			 * override for call the two OnStart of IPhysics and IUpdatable
			 */
			virtual bool OnStart() override;

			virtual void StartCopy(IComponent*& copyTo) const override;

			virtual void OnCopy(IComponent* copyTo) const override;

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

			/** 
			 * Set the linear velocity of the PxRigidDynamic
			 * @param newVelocity: the new velocity
			 */
			inline virtual void SetLinearVelocity(Core::Maths::Vec3 newVelocity);

			/**
			 * Get the linear velocity of the PxRigidDynamic
			 * @return the current velocity
			 */
			virtual Core::Maths::Vec3 GetVelocity();
			
			/**
			 * Add a vector to the linear velocity of the PxRigidDynamic
			 * @param vector: the vector we want to add to the velocity
			 */
			inline virtual void AddVelocity(const Core::Maths::Vec3 vector);

			/**
			 * Set the mass of the PxRigidDynamic
			 * @param mass: the new mass
			 */
			void SetMass(const float mass);

			/**
			 * Get the mass of the PxRigidDynamic
			 * @return the current mass
			 */
			float GetMass();

			/**
			 * Clear all the forces of the PxRigidDynamic
			 */
			virtual void ClearForces();

			/**
			 * Lock or delock the rotation for the axis x,y,z during the physics simulation
			 * @param Axisx: true for lock the axis x
			 * @param Axisy: true for lock the axis y
			 * @param Axisz: true for lock the axis z
			 */
			virtual void PhysicsLockRotation(bool Axisx, bool Axisy, bool Axisz);

			/**
			 * Lock or delock the rotation for the axis x during the physics simulation
			 * @param Axisx: true for lock the axis x
			 */
			virtual void SetPhysicsLockXRotation(bool Axisx);
			/**
			 * Get if the rotation in x is lock or not
			 * @return true if the axis x is lock
			 */
			virtual bool GetPhysicsLockXRotation();

			/**
			 * Lock or delock the rotation for the axis x during the physics simulation
			 * @param Axisx: true for lock the axis y
			 */
			virtual void SetPhysicsLockYRotation(bool Axisy);
			/**
			 * Get if the rotation in x is lock or not
			 * @return true if the axis y is lock
			 */
			virtual bool GetPhysicsLockYRotation();

			/**
			 * Lock or delock the rotation for the axis x during the physics simulation
			 * @param Axisx: true for lock the axis z
			 */
			virtual void SetPhysicsLockZRotation(bool Axisz);
			/**
			 * Get if the rotation in x is lock or not
			 * @return true if the axis z is lock
			 */
			virtual bool GetPhysicsLockZRotation();


			REGISTER_CLASS(Core::Datastructure::ComponentBase, Core::Datastructure::IPhysics, Core::Datastructure::IUpdatable)
		};
	}
}

