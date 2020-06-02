#pragma once

#include "CoreMinimal.h"
#include "ComponentBase.h"
#include "ICollidable.h"
#include "IUpdatable.hpp"
#include "PxRigidDynamic.h"
#include "Vec3.hpp"

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
		struct RigidBodySave
		{
			float	Mass;
			bool	XLock;
			bool	YLock;
			bool	ZLock;
			bool	Gravity;
		};

		/**
		 * Contains a collider and a link to a PxRigidDynamic from the PhysicsScene
		 */
		BAKERS_API_CLASS RigidBody : public Core::Datastructure::ComponentBase, public virtual Core::Datastructure::IUpdatable
		{
		private:
			RigidBodySave*			m_tmpRigidBodySave{nullptr};
			physx::PxRigidDynamic* m_pxRigidBody{ nullptr };
			int						m_IDFunctionSetTRS{ 0 };
			bool					m_BodyChangeGlobalPos{ false };

		protected:

			/**
			 * Function inheritated from ComponentBase and IUpdatable,
			 * override for call the two OnStart of ComponentBase and IUpdatable
			 */
			virtual bool OnStart() override;

			/**
			 * Function inheritated from ComponentBase
			 * override for call the StartCopy of ComponentBase
			 * and init copyTo as an RigidBody
			 */
			virtual void StartCopy(IComponent*& copyTo) const override;

			/**
			 * Function inheritated from ComponentBase
			 * override for call the OnCopy of ComponentBase
			 * and copy the save
			 */
			virtual void OnCopy(IComponent* copyTo) const override;

			/**
			 * Create a save of the PxRigidDynamic
			 * use for OnCopy
			 */
			RigidBodySave* SaveRigidBody() const;

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

			/**
			 * Set the PxRigidDynamic to default
			 */
			void SetToDefault();

		public:
			/**
			 * Default Constructor
			 */
			RigidBody() = default;

			/**
			 * Default Destructor
			 */
			~RigidBody();

			virtual void OnInit() override;

			/**
			 * Function inheritated from IPhysics and IUpdatable,
			 * override for assign the new position and new rotation to the Object
			 * @param deltaTime: Time since previous frame
			 */
			virtual void OnUpdate(float deltaTime) override;

			/**
			 * Init the value of the physics if they are save
			 */
			void InitPhysic();

			/**
			 * Set the position of the PxRigidActor with the position 
			 * of the parent object
			 */
			void SetPhysicsTransformParent();

			/** 
			 * Set the linear velocity of the PxRigidDynamic
			 * @param newVelocity: the new velocity
			 */
			inline virtual void SetLinearVelocity(Core::Maths::Vec3 newVelocity);
			/**
			 * Get the linear velocity of the PxRigidDynamic
			 * @return the current velocity
			 */
			virtual Core::Maths::Vec3 GetVelocity() const;

			/**
			 * Set the angular velocity of the PxRigidDynamic
			 * @param angularVelocity: the new angular velocity
			 */
			virtual void SetAngularVelocity(Core::Maths::Vec3 angularVelocity);
			/**
			 * Get the angular velocity of the PxRigidDynamic
			 * @return the current angular velocity
			 */
			virtual Core::Maths::Vec3 GetAngularVelocity() const;

			
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
			float GetMass() const;

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
			virtual bool GetPhysicsLockXRotation() const;
			/**
			 * Lock or delock the rotation for the axis x during the physics simulation
			 * @param Axisx: true for lock the axis y
			 */
			virtual void SetPhysicsLockYRotation(bool Axisy);
			/**
			 * Get if the rotation in x is lock or not
			 * @return true if the axis y is lock
			 */
			virtual bool GetPhysicsLockYRotation() const;
			/**
			 * Lock or delock the rotation for the axis x during the physics simulation
			 * @param Axisx: true for lock the axis z
			 */
			virtual void SetPhysicsLockZRotation(bool Axisz);
			/**
			 * Get if the rotation in x is lock or not
			 * @return true if the axis z is lock
			 */
			virtual bool GetPhysicsLockZRotation() const;

			/**
			 * Lock or delock the rotation for the axis x during the physics simulation
			 * @param use: true use the gravity
			 */
			virtual void SetUseGravity(bool use);
			/**
			 * Lock or delock the rotation for the axis x during the physics simulation
			 * @return true if the RigidBody use the gravity
			 */
			virtual bool GetUseGravity() const;

			/**
			 * Set lock state of given rotation. Used by lua scripts
			 * @param rotation: Rotation axis, values accepted are 0, 1 and 2 for x, y and z axis
			 * @param lock: New state for rotation lock
			 */
			void SetRotationLock(int rotation, bool lock);

			/**
			 * Put the rigidBody to nullptr do not use it in your own code
			 */
			void ToNull() { m_pxRigidBody = nullptr; }

			REGISTER_CLASS(Core::Datastructure::ComponentBase, Core::Datastructure::IUpdatable)
		};
	}
}

