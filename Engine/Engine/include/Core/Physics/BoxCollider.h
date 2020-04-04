#pragma once
#include "Collider.h"

namespace Core::Physics
{
	class BoxCollider : public Collider
	{
		Core::Maths::Vec3	m_extent{ 0.5f,0.5f,0.5f };
		Core::Maths::Vec3	m_localPosition{ 0.f,0.f,0.f };
	public :
		BoxCollider() = default;
		virtual void CreateShape(physx::PxPhysics* physics) override;

		void SetBoxHalfExtent(const Core::Maths::Vec3& halfExtent);
	};
}
