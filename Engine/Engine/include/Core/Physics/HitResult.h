#pragma once
#include "Vec3.hpp"


namespace Core::Physics
{
	/**
	 * Contains values extract from the PxContactPairPoint
	 */
	struct HitResult
	{
		Core::Maths::Vec3	impactPoint{};
		Core::Maths::Vec3	impactNormal{};
		bool				isPenetrate{ false };
		float				penetrationDepth{0.f};
	};
}
