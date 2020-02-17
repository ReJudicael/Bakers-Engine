#ifndef __MATHS__
#define __MATHS__

#include "Mat.hpp"
#include "Mat4.hpp"
#include "Vec.hpp"
#include "Vec2.hpp"
#include "Vec3.hpp"
#include "Vec4.hpp"
#include "Ref.hpp"
#include "Ref3D.hpp"
#include "Quaternion.hpp"

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
	}
}

#endif
