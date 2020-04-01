#pragma once
#include "Vec3.hpp"


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
		private:
			physx::PxShape*		m_pxShape;
			physx::PxMaterial*	m_pxMaterial;
			Core::Maths::Vec3	m_extent{ 1.f,1.f,1.f };
			Core::Maths::Vec3	m_localPosition{ 0.f,0.f,0.f };
			Core::Maths::Vec3	m_physicsMaterial{ 0.5,0.5,0.1 };
		public:

			physx::PxShape* GetShape()
			{
				return m_pxShape;
			}

			void SetExtent(const Core::Maths::Vec3& vec)
			{
				m_extent = vec;
			}


			void initCube(const Core::Maths::Vec3& extent, const Core::Maths::Vec3& physicsMaterial, const Core::Maths::Vec3& localPosition = Core::Maths::Vec3());
			void createCuceShape(physx::PxPhysics* physics);


		};
	}
}

