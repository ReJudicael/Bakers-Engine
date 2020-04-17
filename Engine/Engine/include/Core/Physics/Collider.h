#pragma once
#include "Vec3.hpp"
#include "Quaternion.hpp"
#include "CoreMinimal.h"


namespace physx
{
	class PxShape;
	class PxPhysics;
	class PxMaterial;
}

namespace Core
{
	namespace Physics
	{
		/**
		 * Contains the shape and the material of the physics mesh 
		 * with wich he is attached
		 */
		BAKERS_API_CLASS Collider
		{
		protected:
			physx::PxShape*		m_pxShape;
			physx::PxMaterial*	m_pxMaterial;
		public:

			/**
			 * Get the PhysX shape of the collider
			 * @return the PhysX shape of the collider 
			 */
			physx::PxShape* GetShape()
			{
				return m_pxShape;
			}

			/**
			 * Set the local position of the shape compared to 
			 * the physics mesh with wich he is attached
			 * @param pos: the local position we want to give to the shape
			 */
			virtual void SetLocalPosition(Core::Maths::Vec3 pos);

			/**
			 * Set the local position of the shape compared to
			 * the physics mesh with wich he is attached
			 * @return the local position of the shape
			 */
			virtual Core::Maths::Vec3 GetLocalPosition();

			/**
			 * Set the local rotation of the shape compared to
			 * the physics mesh with wich he is attached
			 * @param quat: the local rotation we want to give to the shape with quaternion
			 */
			virtual void SetLocalRotationQuat(const Core::Maths::Quat& quat);

			/**
			 * Set the local rotation of the shape compared to
			 * the physics mesh with wich he is attached
			 * @return the local rotation of the shape in quaternion
			 */
			virtual Core::Maths::Quat GetLocalRotationQuat();

			/**
			 * Set the local rotation of the shape compared to
			 * the physics mesh with wich he is attached
			 * @param euler: the local rotation we want to give to the shape with Euler angle
			 */
			virtual void SetLocalRotationEuler(Core::Maths::Vec3 euler);

			/**
			 * Set the local rotation of the shape compared to
			 * the physics mesh with wich he is attached
			 * @return the local rotation in Euler angle
			 */
			virtual Core::Maths::Vec3 GetLocalRotationEuler();

			/**
			 * Set the local rotation of the shape compared to
			 * the physics mesh with wich he is attached
			 * @param vec: the material of the shape, StaticFriction, DynamicFriction, Restitution
			 */
			virtual void SetMaterial(Core::Maths::Vec3 mat);

			/**
			 * Set the local rotation of the shape compared to
			 * the physics mesh with wich he is attached
			 * @return the material of the shape, StaticFriction, DynamicFriction, Restitution
			 */
			virtual Core::Maths::Vec3 GetMaterial();

			/**
			 * create the shape of the collider from the PhysicsScene
			 * @param physics: the PhysX physics from the PhysicsScene
			 */
			virtual void CreateShape(physx::PxPhysics* physics) = 0;

			/**
			 * Set the trigger state
			 * @param trigger: booleen who set to true or false the state trigger
			 */
			virtual void Trigger(bool trigger);

			/**
			 * return the trigger state of the shape
			 * @return true if the shape is a trigger
			 */
			virtual bool IsTrigger();

			/**
			 * Set the state trigger of the shape
			 */
			virtual void TriggerCollider();

			/**
			 * Set the state simulation of the shape
			 */
			virtual void SimulationCollider();

			/*
			 * Destroy the shape and his pxMaterial
			 */
			virtual void DestroyShape();


			~Collider();
			REGISTER_CLASS()
		};
	}
}
