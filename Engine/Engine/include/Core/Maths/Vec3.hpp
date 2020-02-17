#ifndef __VEC3__
#define __VEC3__

#include <type_traits>
#include <cmath>

#include "Vec.hpp"

namespace Core::Maths
{
	/**
	 * Vector class with predefined size of 3
	 */
	struct Vec3
	{
		union
		{
			float		xyz[3];
			struct {
				float	x;
				float	y;
				float	z;
			};
			struct
			{
				Vec<2>	xy;
				float	z;
			};
			struct 
			{
				float	x;
				Vec<2>	yz;
			};
			Vec<3>		vec;
		};
		/**
		 * Default constructor, creates a vec zero
		 */
		inline constexpr		Vec3() noexcept : x {0}, y {0}, z {0}
		{
		}

		/**
		 * Creates a vector with all its values initialized at
		 * the given value
		 * @param f: Value to witch vector is initialized
		 */
		inline constexpr		Vec3(const float f) noexcept : x {f}, y {f}, z {f}
		{
		}

		/**
		 * Creates a copy of given vector
		 * @param v: Vector to be copied
		 */
		inline constexpr		Vec3(const Vec3& v) noexcept : x {v.x}, y {v.y}, z {v.z}
		{
		}

		/**
		 * Creates a copy of given templated vector
		 * @param v: Templated vector to be copied
		 */
		inline constexpr		Vec3(const Vec<3> & v) noexcept : x{ v[0] }, y{ v[1] }, z{ v[2] }
		{
		}
		/**
		 * Creates a vector with the given values
		 * @param X: The X component of the vector
		 * @param Y: The Y component of the vector
		 * @param Z: The Z component of the vector
		 */
		inline constexpr		Vec3(const float X, const float Y = 0.f, const float Z = 0.f) noexcept : x {X}, y {Y}, z {Z}
		{
		}

		/**
		 * Moves the given vector to a new vector
		 * @param v: Vector to be moved
		 */
		inline constexpr		Vec3(Vec3&& v) noexcept : x {std::move(v.x)}, y {std::move(v.y)}, z {std::move(v.z)}
		{
		}

		/**
		 * Moves the given templated vector to a new vector
		 * @param v: Templated vector to be moved
		 */
		inline constexpr		Vec3(Vec<3>&& v) noexcept : x {std::move(v[0])}, y {std::move(v[1])}, z {std::move(v[2])}
		{
		}

		/**
		 * Creates a vector from a vector matrix
		 * @param m: Matrix to extract vector from
		 */
		inline constexpr		Vec3(const Mat<3, 1> & m) noexcept : x{ m(0) }, y{ m(1) }, z{ m(2) }
		{
		}

		/**
		 * Creates a vector from a vector matrix
		 * @param m: Matrix to extract vector from
		 */
		inline constexpr		Vec3(const Mat<4, 1> & m) noexcept : x{ m(0) }, y{ m(1) }, z{ m(2) }
		{
		}
			
		/**
		 * Computes the normalized version of current
		 * vector
		 * @return The normalized version of current vector
		 */
		inline constexpr Vec3		Normalized() const noexcept
		{
			return *this / Length(); 
		}		

		/**
		* Normalizes the current vector
		*/
		inline constexpr void		Normalize() noexcept
		{
			*this /= Length();
		}

		/**
		 * Computes the square length of the vector
		 * @return The square length of the vector
		 */
		inline constexpr float		SquaredLength() const noexcept
		{
			return (x * x + y * y + z * z);
		}

		/**
		 * Computes the length of the vector
		 * @return The length of the vector
		 */
		inline float				Length() const noexcept
		{
			return sqrt(SquaredLength());
		}
	
		/**
		 * Computes the dot product of the current
		 * vector with given vector
		 * @param v: Vector to dot product with
		 * @return The result of the dot product
		 */
		inline constexpr float		Dot(const Vec3& v) const noexcept
		{
			return (x * v.x + y * v.y + z * v.z);
		}

		/**
		 * Computes the cross product of the current
		 * vector with given vector
		 * @param v: Vector to cross product with
		 * @return The result of the cross product
		 */
		inline constexpr Vec3		Cross(const Vec3& v) const noexcept
		{
			Vec3	newVec {};	
			newVec.x = y * v.z - z * v.y;
			newVec.y = z * v.x - x * v.z;
			newVec.z = x * v.y - y * v.x;
			return newVec;
		}

		/**
		 * Copies the given vector in current vector
		 * @param v: Vector to copy
		 * @return A copy of the given vector
		 */
		inline constexpr Vec3	operator= (const Vec3& v) noexcept
		{
			x = v.x;
			y = v.y;
			z = v.z;

			return *this;
		}

		/**
		 * Moves the given vector in the current vector
		 * @param v: Vector to move
		 * @return A copy of the given vector
		 */
		inline constexpr Vec3	operator= (Vec3&& v) noexcept
		{
			x = std::move(v.x);
			y = std::move(v.y);
			z = std::move(v.z);
			return *this;
		}
		
		/**
		 * Compares the given vector to the current vector
		 * @param v: Vector to compare to
		 * @return The result of the comparison
		 */
		inline constexpr bool		operator== (const Vec3& v) const noexcept
		{
			return (x == v.x && y == v.y && z == v.z);
		}
	
		/**
		 * Compares the given vector to the current vector
		 * @param v: Vector to compare to
		 * @return The result of the comparison
		 */
		inline constexpr bool		operator!= (const Vec3& v) const noexcept
		{
			return (x != v.x && y != v.y && z != v.z);
		}
	
		/**
		 * Computes the addition of the current vector to
		 * the given vector
		 * @param v: Vector to add
		 * @return The result of the addition
		 */
		inline constexpr Vec3		operator+ (const Vec3& v) const noexcept
		{
			Vec3	newVec {*this};
			newVec.x += v.x;
			newVec.y += v.y;
			newVec.z += v.z;

			return newVec;
		}

		/**
		 * Computes the substaction of the given vector
		 * to the current vector
		 * @param v: Vector to substract
		 * @return The result of the substaction
		 */
		inline constexpr Vec3		operator- (const Vec3& v) const noexcept
		{	
			Vec3	newVec {*this};
			newVec.x -= v.x;
			newVec.y -= v.y;
			newVec.z -= v.z;

			return newVec;
		}
			
		/**
		 * Computes the multiplication of the components
		 * of the current vector by the respective components 
		 * of the given vector
		 * @param v: Vector to myltiply by
		 * @return The result of the multiplication
		 */
		inline constexpr Vec3		operator* (const Vec3& v) const noexcept
		{
			Vec3	newVec {v};
			newVec.x *= x;
			newVec.y *= y;
			newVec.z *= z;
			return newVec;
		}

		/**
		 * Computes the division of the components
		 * of the current vector by the respective components
		 * of the given vector
		 * @param v: Vector to diviside by
		 * @return The result of the division
		 */
		inline constexpr Vec3		operator/ (const Vec3& v) const noexcept
		{
			Vec3	newVec {*this};
			newVec.x = x / v.x;
			newVec.y = y / v.y;
			newVec.z = z / v.z;

			return newVec;
		}

		/**
		 * Computes the division of the components
		 * of the current vector by the given scalar
		 * @param f: Scalar to divide by
		 * @return The result of the scalar division
		 */
		inline constexpr Vec3		operator/ (const float f) const noexcept
		{
			Vec3	newVec {*this};
			newVec.x = x / f;
			newVec.y = y / f;
			newVec.z = z / f;
			return newVec;
		}
	
		/**
		 * Adds given vector to current vector
		 * @param v: Vector to add
		 * @return Copy of the result
		 */
		inline constexpr Vec3		operator+= (const Vec3& v) noexcept
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}
	
		/**
		 * Substracts given vector to current vector
		 * @param v: Vector to substract
		 * @return Copy of the result
		 */
		inline constexpr Vec3		operator-= (const Vec3& v) noexcept
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}
	
		/**
		 * Multiply given vector to current vector
		 * @param v: Vector to multiply
		 * @return Copy of the result
		 */
		inline constexpr Vec3		operator*= (const Vec3& v) noexcept
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			return *this;
		}

		/**
		 * Divides current vector by given scalar
		 * @param f: Scalar to divide by
		 * @return Copy of the result
		 */
		inline constexpr Vec3		operator/= (const float f) noexcept
		{
			x /= f;
			y /= f;
			z /= f;
			return *this;
		}
	
		/** 
		 * Divides current vector by given vector
		 * @param v: Vector to divide by
		 * @return Copy of the result
		 */
		inline constexpr Vec3		operator/= (const Vec3& v) noexcept
		{
			x /= v.x;
			y /= v.y;
			z /= v.z;
			return *this;
		}

		/**
		 * Converts vector into templated vector
		 */
		inline constexpr			operator Vec<3>() const noexcept
		{
			float	f[3]{ x, y, z };
			return Vec<3>(f);
		}

		/** 
		 * Converts vector into column matrix
		 */
		inline constexpr			operator Mat<3, 1>() const noexcept
		{
			Mat<3, 1> m;

			m.Set(0, x);
			m.Set(1, y);
			m.Set(2, z);

			return m;
		}

		/**
		 * Converts vector into column matrix
		 */
		inline constexpr			operator Mat<4, 1>() const noexcept
		{
			Mat<4, 1> m;

			m.Set(0, x);
			m.Set(1, y);
			m.Set(2, z);
			m.Set(3, 1.f);

			return m;
		}

		/**
		 * Converts vector into line matrix
		 */
		inline constexpr			operator Mat<1, 3>() const noexcept
		{
			Mat<1, 3> m;

			m.Set(0, x);
			m.Set(1, y);
			m.Set(2, z);

			return m;
		}

		/**
		 * Converts vector into line matrix
		 */
		inline constexpr			operator Mat<1, 4>() const noexcept
		{
			Mat<1, 4> m;

			m.Set(0, x);
			m.Set(1, y);
			m.Set(2, z);
			m.Set(3, 1.f);

			return m;
		}

		/**
		 * Converts vector to string format
		 * @return String version of the current vector
		 */
		inline std::string			ToString() const noexcept
		{
			return std::to_string(x) + std::to_string(y) + std::to_string(z);
		}
	};
}

#endif
