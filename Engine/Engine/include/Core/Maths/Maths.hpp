#ifndef __MATHS__
#define __MATHS__

#include "MathMinimal.h"

#include "Mat.hpp"
#include "Mat4.hpp"
#include "Vec.hpp"
#include "Vec2.hpp"
#include "Vec3.hpp"
#include "Vec4.hpp"
#include "Ref.hpp"
#include "Ref3D.hpp"
#include "Quaternion.hpp"

constexpr double RAD2DEG = 180.f / M_PI;
constexpr double DEG2RAD = M_PI / 180.f;

namespace physx
{
	class PxVec3;
	class PxQuat;
}

/**
 * Contains the core of the engine
 */
namespace Core
{
	/**
	 * Contains all maths classes and structs of the engine
	 */
	namespace Maths
	{
		/**
		 * Templated clamp function
		 * @tparam T: Type of the value to clamp
		 * @param value: Value to clamp
		 * @param upper: Upper bound of the clamp
		 * @param lower: Lower bound of the clamp
		 * @return The clamped value
		 */
		template <typename T>
		inline constexpr T	Clamp(const T& value, const T& upper, const T& lower) noexcept
		{
			if (value > upper)
				return upper;
			if (value < lower)
				return lower;
			return value;
		}

		/**
		 * Templated max function
		 * @tparam T: Type of the values to compare
		 * @param v1: First value to compare
		 * @param v2: Second value to compare
		 * @return The biggest of the two values
		 */
		template <typename T>
		inline constexpr T& Max(const T& v1, const T& v2)
		{
			if (v1 > v2)
				return v1;
			return v2;
		}

		/**
		 * Templated min function
		 * @tparam T: Type of the values to compare
		 * @param v1: First value to compare
		 * @param v2: Second value to compare
		 * @return The smallest of the two values
		 */
		template <typename T>
		inline constexpr T& Min(const T& v1, const T& v2)
		{
			if (v1 < v2)
				return v1;
			return v2;
		}

		/**
		 * Return given angle in radians
		 * @param angle: Angle in degrees
		 */
		inline constexpr double ToRadians(const double& angle)
		{
			return angle * DEG2RAD;
		}

		/**
		 * Return given angle in degrees
		 * @param angle: Angle in radians
		 */
		inline constexpr double ToDegrees(const double& angle)
		{
			return angle * RAD2DEG;
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
}

#endif
