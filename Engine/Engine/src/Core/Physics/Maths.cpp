#include "Maths.hpp"
#include "PxVec3.h"

namespace Core::Maths
{
	physx::PxVec3 operator+(physx::PxVec3 pxV, Vec3 v)
	{
		return { pxV.x + v.x, pxV.y + v.y, pxV.z + v.z };
	}

}
