#pragma once
#include "Vec3.hpp"
#include "Quaternion.hpp"


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
		class Collider
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
			 * @param vec: the local position we want to give to the shape
			 */
			virtual void SetLocalPosition(const Core::Maths::Vec3& vec);

			/**
			 * Set the local rotation of the shape compared to
			 * the physics mesh with wich he is attached
			 * @param quat: the local rotation we want to give to the shape
			 */
			virtual void SetLocalRotation(const Core::Maths::Quat& quat);

			/**
			 * create the shape of the collider from the PhysicsScene
			 * @param physics: the PhysX physics from the PhysicsScene
			 */
			virtual void CreateShape(physx::PxPhysics* physics) = 0;

			virtual void DestroyShape();

			~Collider();


		};
	}
}

