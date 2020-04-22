#include "Maths.hpp"
#include "CoreMinimal.h"

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<Core::Maths::Vec2>("Vec2")
		.constructor()
		.property("x", &Core::Maths::Vec2::x)
		.property("y", &Core::Maths::Vec2::y);

	rttr::registration::class_<Core::Maths::Vec3>("Vec3")
		.constructor()
		.property("x", &Core::Maths::Vec3::x)
		.property("y", &Core::Maths::Vec3::y)
		.property("z", &Core::Maths::Vec3::z);

	rttr::registration::class_<Core::Maths::Vec4>("Vec4")
		.constructor()
		.property("x", &Core::Maths::Vec4::x)
		.property("y", &Core::Maths::Vec4::y)
		.property("z", &Core::Maths::Vec4::z)
		.property("w", &Core::Maths::Vec4::w);

	rttr::registration::class_<Core::Maths::Color>("Color")
		.constructor()
		.property("r", &Core::Maths::Color::r)
		.property("g", &Core::Maths::Color::g)
		.property("b", &Core::Maths::Color::b)
		.property("a", &Core::Maths::Color::a);
}

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
