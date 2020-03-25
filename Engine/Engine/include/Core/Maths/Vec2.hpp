#ifndef __VEC2__
#define __VEC2__

#include "MathMinimal.h"

#include <type_traits>

#include "Vec.hpp"

namespace Core::Maths
{
	/**
	 * Vector class with predefined size of 2
	 */
	struct Vec2
	{
		union
		{
			struct {
				float x;
				float y;
			};
			Vec<2>	vec;
			float	xy[2];
		};

		/**
		 * Default constructor, creates a vec zero
		 */
		inline constexpr		Vec2() noexcept;

		/**
		 * Creates a vector with all its values initialized at
		 * the given value
		 * @param f: Value to witch vector is initialized
		 */
		inline constexpr		Vec2(const float f) noexcept;

		/**
		 * Creates a copy of given vector
		 * @param v: Vector to be copied
		 */
		inline constexpr		Vec2(const Vec2& v) noexcept;

		/**
		 * Creates a vector with the given values
		 * @param X: The X component of the vector
		 * @param Y: The Y component of the vector
		 */
		inline constexpr		Vec2(const float X, const float Y) noexcept;

		/**
		 * Moves the given vector to a new vector
		 * @param v: Vector to be moved
		 */
		inline constexpr		Vec2(Vec2&& v) noexcept;

		/**
		 * Moves the given templated vector to a new vector
		 * @param v: Templated vector to be moved
		 */
		inline constexpr		Vec2(Vec<2> && v) noexcept;

		
		/**
		 * Computes the normalized version of current
		 * vector
		 * @return The normalized version of current vector
		 */
		inline Vec2				Normalized() const noexcept;

		/**
		* Normalizes the current vector
		*/
		inline void				Normalize() noexcept;

		/**
		 * Computes the square length of the vector
		 * @return The square length of the vector
		 */
		inline constexpr float	SquaredLength() const noexcept;

		/**
		 * Computes the length of the vector
		 * @return The length of the vector
		 */
		inline float			Length() const noexcept;

		/**
		 * Copies the given vector in current vector
		 * @param v: Vector to copy
		 * @return A copy of the given vector
		 */
		inline constexpr Vec2	operator= (const Vec2& v) noexcept;

		/**
		 * Moves the given vector in the current vector
		 * @param v: Vector to move
		 * @return A copy of the given vector
		 */
		inline constexpr Vec2	operator= (Vec2&& v) noexcept;

		/**
		 * Compares the given vector to the current vector
		 * @param v: Vector to compare to
		 * @return The result of the comparison
		 */
		inline constexpr bool		operator== (const Vec2& v) const noexcept;

		/**
		 * Compares the given vector to the current vector
		 * @param v: Vector to compare to
		 * @return The result of the comparison
		 */
		inline constexpr bool		operator!= (const Vec2& v) const noexcept;

		/**
		 * Computes the addition of the current vector to
		 * the given vector
		 * @param v: Vector to add
		 * @return The result of the addition
		 */
		inline constexpr Vec2		operator+ (const Vec2& v) const noexcept;

		/**
		 * Computes the substaction of the given vector
		 * to the current vector
		 * @param v: Vector to substract
		 * @return The result of the substaction
		 */
		inline constexpr Vec2		operator- (const Vec2& v) const noexcept;

		/**
		 * Computes the division of the components
		 * of the current vector by the given scalar
		 * @param f: Scalar to divide by
		 * @return The result of the scalar division
		 */
		inline constexpr Vec2		operator/ (const float f) const noexcept;

		/**
		 * Adds given vector to current vector
		 * @param v: Vector to add
		 * @return Copy of the result
		 */
		inline constexpr Vec2		operator+= (const Vec2& v) noexcept;

		/**
		 * Substracts given vector to current vector
		 * @param v: Vector to substract
		 * @return Copy of the result
		 */
		inline constexpr Vec2		operator-= (const Vec2& v) noexcept;

		/**
		 * Divides current vector by given scalar
		 * @param f: Scalar to divide by
		 * @return Copy of the result
		 */
		inline constexpr Vec2		operator/= (const float f) noexcept;

		/**
		 * Converts vector to string format
		 * @return String version of the current vector
		 */
		inline std::string			ToString() const noexcept
		{
			std::string s;
			s += std::to_string(x) + " ";
			s += std::to_string(y);
			
			return s;
		}

		/**
		 * Print current vector to the console
		 */
		inline void					Print() const noexcept
		{
			std::cout << ToString() << std::endl;
		}
	};

	inline constexpr Core::Maths::Vec2::Vec2() noexcept : x {0}, y {0}
	{
	}
	inline constexpr Vec2::Vec2(const float f) noexcept : x {f}, y {f}
	{
	}
	inline constexpr Vec2::Vec2(const Vec2& v) noexcept : x {v.x}, y {v.y}
	{
	}
	inline constexpr Vec2::Vec2(const float X, const float Y) noexcept : x {X}, y {Y}
	{
	}
	inline constexpr Vec2::Vec2(Vec2&& v) noexcept : x {std::move(v.x)}, y {std::move(v.y)}
	{
	}
	inline constexpr Vec2::Vec2(Vec<2> && v) noexcept : x {v[0]}, y {v[1]}
	{
	}
	inline Vec2		Vec2::Normalized() const noexcept
	{
		float len{ Length() };
		return *this / len;
	}
	inline void		Vec2::Normalize() noexcept
	{
		float len{ Length() };
		*this /= len;
	}
	inline constexpr float Vec2::SquaredLength() const noexcept
	{
		return x * x + y * y;
	}
	inline float Vec2::Length() const noexcept
	{
		return sqrt(SquaredLength());
	}
	inline constexpr Vec2 Vec2::operator=(const Vec2& v) noexcept
	{
		x = v.x;
		y = v.y;

		return *this;
	}
	inline constexpr Vec2 Vec2::operator=(Vec2&& v) noexcept
	{
		x = std::move(v.x);
		y = std::move(v.y);

		return *this;
	}
	inline constexpr bool Vec2::operator==(const Vec2& v) const noexcept
	{
		return x == v.x && y == v.y;
	}
	inline constexpr bool Vec2::operator!=(const Vec2& v) const noexcept
	{
		return !(*this == v);
	}
	inline constexpr Vec2 Vec2::operator+(const Vec2& v) const noexcept
	{
		return Vec2(x + v.x, y + v.y);
	}
	inline constexpr Vec2 Vec2::operator-(const Vec2& v) const noexcept
	{
		return Vec2(x - v.x, y - v.y);
	}
	inline constexpr Vec2 Vec2::operator/(const float f) const noexcept
	{
		return Vec2(x / f, y / f);
	}
	inline constexpr Vec2 Vec2::operator+=(const Vec2& v) noexcept
	{
		x += v.x;
		y += v.y;
		return *this;
	}
	inline constexpr Vec2 Vec2::operator-=(const Vec2& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}
	inline constexpr Vec2 Vec2::operator/=(const float f) noexcept
	{
		x /= f;
		y /= f;
		return *this;
	}
}

#endif
