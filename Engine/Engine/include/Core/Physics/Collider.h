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
		class Collider
		{
		protected:
			physx::PxShape*		m_pxShape;
			physx::PxMaterial*	m_pxMaterial;
		public:

			physx::PxShape* GetShape()
			{
				return m_pxShape;
			}


			virtual void SetLocalPosition(const Core::Maths::Vec3& vec);

			virtual void SetLocalRotation(const Core::Maths::Quat& quat);

			void initCube(const Core::Maths::Vec3& extent, const Core::Maths::Vec3& physicsMaterial, const Core::Maths::Vec3& localPosition = Core::Maths::Vec3());
			void createCuceShape(physx::PxPhysics* physics);
			virtual void CreateShape(physx::PxPhysics* physics) = 0;


		};
	}
}

