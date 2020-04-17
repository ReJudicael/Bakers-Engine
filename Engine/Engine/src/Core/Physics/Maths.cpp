#include "Maths.hpp"
#include "PxVec3.h"

#include <sol.hpp>

namespace Core::Maths
{
	physx::PxVec3 operator+(physx::PxVec3 pxV, Vec3 v)
	{
		return { pxV.x + v.x, pxV.y + v.y, pxV.z + v.z };
	}

	void RegisterMathsForLua(sol::state& lua)
	{
		lua.new_usertype<Vec2>("Vec2",
			sol::constructors<Vec2(), Vec2(float, float)>(),
			"x", &Vec2::x,
			"y", &Vec2::y,
			"xy", &Vec2::xy,
			"Normalize", &Vec2::Normalize,
			"SquaredLength", &Vec2::SquaredLength);

		lua.new_usertype<Vec3>("Vec3",
			sol::constructors<Vec3(), Vec3(float, float, float)>(),
			"x", &Vec3::x,
			"y", &Vec3::y,
			"z", &Vec3::z,
			"xyz", &Vec3::xyz,
			"Normalize", &Vec3::Normalize,
			"SquaredLength", &Vec3::SquaredLength,
			"Dot", &Vec3::Dot,
			"Cross", &Vec3::Cross);

		lua.new_usertype<Vec4>("Vec4",
			sol::constructors<Vec4(), Vec4(float, float, float, float)>(),
			"x", &Vec4::x,
			"y", &Vec4::y,
			"z", &Vec4::z,
			"w", &Vec4::w,
			"xyzw", &Vec4::xyzw,
			"Normalize", &Vec4::Normalize,
			"SquaredLenght", &Vec4::SquaredLength,
			"Dot", sol::resolve<float(const Vec4&) const>(&Vec4::Dot),
			"Cross", sol::resolve<Vec4(const Vec4&) const>(&Vec4::Cross));
	}

}
