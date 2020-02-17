#ifndef __VEC4__
#define __VEC4__

#include <type_traits>
#include <cmath>

#include "Vec.hpp"
#include "Vec3.hpp"

namespace Core::Maths
{
	/**
	 * Vector class with predefined size of 4
	 */
	struct Vec4
	{
		union
		{
			float		xyzw[4];
			struct
			{
				float	x;
				float	y;
				float	z;
				float	w;
			};
			struct
			{
				Vec<2>	xy;
				Vec<2>	zw;
			};
			struct
			{
				float	x;
				Vec3	yzw;
			};
			struct
			{
				Vec3	xyz;
				float	w;
			};
			Vec<4>		vec;
		};
		/**
		 * Default constructor, creates a vec zero
		 */
		inline constexpr		Vec4() noexcept : x {0}, y {0}, z {0}, w {0}
		{
		}

		/**
		 * Creates a vector with all its values initialized at
		 * the given value
		 * @param f: Value to witch vector is initialized
		 */
		inline constexpr		Vec4(const float f) noexcept  : x {f}, y {f}, z {f}, w {f}
		{
		}

		/**
		 * Creates a copy of given vector
		 * @param v: Vector to be copied
		 */
		inline constexpr		Vec4(const Vec4& v) noexcept : x {v.x}, y {v.y}, z {v.z}, w {v.w}
		{
		}

		/**
		 * Creates a copy of given templated vector
		 * @param v: Templated vector to be copied
		 */
		inline constexpr		Vec4(const Vec<4>& v) noexcept : x {v[0]}, y {v[1]}, z {v[2]}, w {v[3]}
		{
		}

		/**
		 * Creates a vector from a vector matrix
		 * @param m: Matrix to extract vector from
		 */
		inline constexpr		Vec4(const Mat<4, 1> & m) noexcept : x{ m(0) }, y{ m(1) }, z{ m(2) }, w{ m(3) }
		{
		}

		/**
		 * Creates a vector from a vector matrix
		 * @param m: Matrix to extract vector from
		 */
		inline constexpr		Vec4(const Mat<3, 1> & m) noexcept : x{ m(0) }, y{ m(1) }, z{ m(2) }, w{ 1.f }
		{
		}
	
		/**
		 * Creates a vector with the given values
		 * @param X: The X component of the vector
		 * @param Y: The Y component of the vector
		 * @param Z: The Z component of the vector
		 * @param W: The W component of the vector
		 */
		inline constexpr		Vec4(const float X, const float Y = 0.f, const float Z = 0.f, const float W = 0.f) noexcept : x {X}, y {Y}, z {Z}, w {W}
		{
		}

		/**
		 * Moves the given vector to a new vector
		 * @param v: Vector to be moved
		 */
		inline constexpr		Vec4(Vec4&& v) noexcept : x {std::move(v.x)}, y {std::move(v.y)}, z {std::move(v.z)}, w {std::move(v.w)}
		{
		}

		/**
		 * Moves the given templated vector to a new vector
		 * @param v: Templated vector to be moved
		 */
		inline constexpr		Vec4(Vec<4>&& v) noexcept : x {std::move(v[0])}, y {std::move(v[1])}, z {std::move(v[2])}, w {std::move(v[3])}
		{
		}
		
		/**
		 * Creates a vector from a smaller given vector and
		 * a given value
		 * @param v: Vector forming the first components of the vector
		 * @param f: Value forming the last component of the vector
		 */
		inline constexpr		Vec4(const Vec3& v, const float& f) noexcept : x{ v.x }, y{ v.y }, z{ v.z }, w{ f }
		{
		}

		/**
		 * Creates a vector from a smaller given templated vector and
		 * a given value
		 * @param v: Templated vector forming the first components of the vector
		 * @param f: Value forming the last component of the vector
		 */
		inline constexpr		Vec4(const Vec<3>& v, const float& f) noexcept : x {v[0]}, y {v[1]}, z {v[2]}, w {f}
		{
		}
		
		/**
		 * Computes the normalized version of current
		 * vector
		 * @return The normalized version of current vector
		 */
		inline constexpr Vec4		Normalized() const noexcept
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
		 * Computes the length of the vector
		 * @return The length of the vector
		 */
		inline float		Length() const noexcept
		{
			return sqrt(SquaredLength());
		}

		/**
		 * Computes the square length of the vector
		 * @return The square length of the vector
		 */
		inline constexpr float		SquaredLength() const noexcept
		{
			return (x * x + y * y + z * z + w * w);
		}

		/**
		 * Computes the dot product of the current
		 * vector with given templated vector
		 * @param v: Templated vector to dot product with
		 * @return The result of the dot product
		 */
		inline constexpr float		Dot(const Vec<4>& v) const noexcept
		{
			return (x * v[0] + y * v[1] + z * v[2] + w * v[3]);
		}

		/**
		 * Computes the cross product of the current
		 * vector with given templated vector
		 * @param v: Templated vector to cross product with
		 * @return The result of the cross product
		 */
		inline constexpr Vec4		Cross(const Vec<4>& v) const noexcept
		{
			Vec4	newVec {};
			newVec.x = y * v[2] - z * v[1];
			newVec.y = z * v[3] - w * v[2];
			newVec.z = w * v[0] - x * v[3];
			newVec.w = x * v[1] - y * v[0];
			return newVec;
		}

		/**
		 * Computes the dot product of the current
		 * vector with given vector
		 * @param v: Vector to dot product with
		 * @return The result of the dot product
		 */
		inline constexpr float		Dot(const Vec4& v) const noexcept
		{
			return (x * v.x + y * v.y + z * v.z + w * v.w);
		}

		/**
		 * Computes the cross product of the current
		 * vector with given vector
		 * @param v: Vector to cross product with
		 * @return The result of the cross product
		 */
		inline constexpr Vec4		Cross(const Vec4& v) const noexcept
		{
			Vec4	newVec {};	
			newVec.x = y * v.z - z * v.y;
			newVec.y = z * v.w - w * v.z;
			newVec.z = w * v.x - x * v.w;
			newVec.w = x * v.y - y * v.x;
			return newVec;
		}

		/**
		 * Copies the given templated vector in current vector
		 * @param v: Templated vector to copy
		 * @return A copy of the given vector
		 */
		inline constexpr Vec4	operator= (const Vec<4>& v) noexcept
		{
			x = v[0];
			y = v[1];
			z = v[2];
			w = v[3];

			return *this;
		}

		/**
		 * Copies the given vector in current vector
		 * @param v: Vector to copy
		 * @return A copy of the given vector
		 */
		inline constexpr Vec4	operator= (const Vec4& v) noexcept
		{
			x = v.x;
			y = v.y;
			z = v.z;
			w = v.w;

			return *this;
		}

		/**
		 * Moves the given templated vector in the current vector
		 * @param v: Templated vector to move
		 * @return A copy of the given vector
		 */
		inline constexpr Vec4	operator= (Vec<4>&& v) noexcept
		{
			x = std::move(v[0]);
			y = std::move(v[1]);
			z = std::move(v[2]);
			w = std::move(v[3]);
			return *this;
		}

		/**
		 * Moves the given vector in the current vector
		 * @param v: Vector to move
		 * @return A copy of the given vector
		 */
		inline constexpr Vec4	operator= (Vec4&& v) noexcept
		{
			x = std::move(v.x);
			y = std::move(v.y);
			z = std::move(v.z);
			w = std::move(v.w);
			return *this;
		}

		/**
		 * Converts vector into templated vector
		 */
		inline constexpr operator Vec<4> () noexcept
		{
			return Vec<4>(*this);
		}
		
		/**
		 * Compares the given vector to the current vector
		 * @param v: Vector to compare to
		 * @return The result of the comparison
		 */
		inline constexpr bool		operator== (const Vec4& v) const noexcept
		{
			return (x == v.x && y == v.y && z == v.z && w == v.w);
		}
	
		/**
		 * Compares the given vector to the current vector
		 * @param v: Vector to compare to
		 * @return The result of the comparison
		 */
		inline constexpr bool		operator!= (const Vec4& v) const noexcept
		{
			return (x != v.x && y != v.y && z != v.z && w != v.w);
		}


		/**
		 * Compares the given templatedvector to the current vector
		 * @param v: Templated vector to compare to
		 * @return The result of the comparison
		 */
		inline constexpr bool		operator== (const Vec<4>& v) const noexcept
		{
			return (x == v[0] && y == v[1] && z == v[2] && w == v[3]);
		}
	
		/**
		 * Compares the given templated vector to the current vector
		 * @param v: Templated vector to compare to
		 * @return The result of the comparison
		 */
		inline constexpr bool		operator!= (const Vec<4>& v) const noexcept
		{
			return (x != v[0] && y != v[1] && z != v[2] && w != v[3]);
		}

		/**
		 * Computes the addition of the current vector to
		 * the given templated vector
		 * @param v: Templated vector to add
		 * @return The result of the addition
		 */
		inline constexpr Vec4		operator+ (const Vec<4>& v) const noexcept
		{
			Vec4	newVec {*this};
			newVec.x += v[0];
			newVec.y += v[1];
			newVec.z += v[2];
			newVec.w += v[3];

			return newVec;
		}

		/**
		 * Computes the addition of the current vector to
		 * the given vector
		 * @param v: Vector to add
		 * @return The result of the addition
		 */
		inline constexpr Vec4		operator+ (const Vec4& v) const noexcept
		{
			Vec4	newVec {*this};
			newVec.x += v.x;
			newVec.y += v.y;
			newVec.z += v.z;
			newVec.w += v.w;

			return newVec;
		}

		/**
		 * Computes the substaction of the given templated vector
		 * to the current vector
		 * @param v: Templated vector to substract
		 * @return The result of the substaction
		 */
		inline constexpr Vec4		operator- (const Vec<4>& v) const noexcept
		{
			Vec4	newVec {*this};
			newVec.x -= v[0];
			newVec.y -= v[1];
			newVec.z -= v[2];
			newVec.w -= v[3];

			return newVec;
		}

		/**
		 * Computes the substaction of the given vector
		 * to the current vector
		 * @param v: Vector to substract
		 * @return The result of the substaction
		 */
		inline constexpr Vec4		operator- (const Vec4& v) const noexcept
		{	
			Vec4	newVec {*this};
			newVec.x -= v.x;
			newVec.y -= v.y;
			newVec.z -= v.z;
			newVec.w -= v.w;

			return newVec;
		}

		/**
		 * Computes the multiplication of the components
		 * of the current vector by the respective components
		 * of the given templated vector
		 * @param v: Templated vector to myltiply by
		 * @return The result of the multiplication
		 */
		inline constexpr Vec4		operator* (const Vec<4>& v) const noexcept
		{
			Vec4	newVec {v};
			newVec.x *= x;
			newVec.y *= y;
			newVec.z *= z;
			newVec.w *= w;
			return newVec;
		}
		
		/**
		 * Computes the multiplication of the components
		 * of the current vector by the respective components
		 * of the given vector
		 * @param v: Vector to myltiply by
		 * @return The result of the multiplication
		 */
		inline constexpr Vec4		operator* (const Vec4& v) const noexcept
		{
			Vec4	newVec {v};
			newVec.x *= x;
			newVec.y *= y;
			newVec.z *= z;
			newVec.w *= w;
			return newVec;
		}
		
		/**
		 * Computes the division of the components
		 * of the current vector by the respective components
		 * of the given templated vector
		 * @param v: Templated vector to diviside by
		 * @return The result of the division
		 */
		inline constexpr Vec4		operator/ (const Vec<4>& v) const noexcept
		{
			Vec4	newVec {};
			newVec.x = x / v[0];
			newVec.y = y / v[1];
			newVec.z = z / v[2];
			newVec.w = w / v[3];

			return newVec;
		}

		/**
		 * Computes the division of the components
		 * of the current vector by the respective components
		 * of the given vector
		 * @param v: Vector to diviside by
		 * @return The result of the division
		 */
		inline constexpr Vec4		operator/ (const Vec4& v) const noexcept
		{
			Vec4	newVec {*this};
			newVec.x = x / v.x;
			newVec.y = y / v.y;
			newVec.z = z / v.z;
			newVec.w = w / v.w;

			return newVec;
		}

		/**
		 * Computes the division of the components
		 * of the current vector by the given scalar
		 * @param f: Scalar to divide by
		 * @return The result of the scalar division
		 */
		inline constexpr Vec4		operator/ (const float f) const noexcept
		{
			Vec4	newVec {*this};
			newVec.x = x / f;
			newVec.y = y / f;
			newVec.z = z / f;
			newVec.w = w / f;
			return newVec;
		}

		/**
		 * Adds given templated vector to current vector
		 * @param v: Templated vector to add
		 * @return Copy of the result
		 */
		inline constexpr Vec4		operator+= (const Vec<4>& v) noexcept
		{
			x += v[0];
			y += v[1];
			z += v[2];
			w += v[3];
			return *this;
		}
	
		/**
		 * Adds given vector to current vector
		 * @param v: Vector to add
		 * @return Copy of the result
		 */
		inline constexpr Vec4		operator+= (const Vec4& v) noexcept
		{
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
			return *this;
		}

		/**
		 * Substracts given templated vector to current vector
		 * @param v: Templated vector to substract
		 * @return Copy of the result
		 */
		inline constexpr Vec4		operator-= (const Vec<4>& v) noexcept
		{
			x -= v[0];
			y -= v[1];
			z -= v[2];
			w -= v[3];
			return *this;
		}
	
		/**
		 * Substracts given vector to current vector
		 * @param v: Vector to substract
		 * @return Copy of the result
		 */
		inline constexpr Vec4		operator-= (const Vec4& v) noexcept
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
			return *this;
		}

		/**
		 * Multiply given templated vector to current vector
		 * @param v: Templated vector to multiply
		 * @return Copy of the result
		 */
		inline constexpr Vec4		operator*= (const Vec<4>& v) noexcept
		{
			x *= v[0];
			y *= v[1];
			z *= v[2];
			w *= v[3];
			return *this;
		}
	
		/**
		 * Multiply given vector to current vector
		 * @param v: Vector to multiply
		 * @return Copy of the result
		 */
		inline constexpr Vec4		operator*= (const Vec4& v) noexcept
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			w *= v.w;
			return *this;
		}

		/**
		 * Divides current vector by given scalar
		 * @param f: Scalar to divide by
		 * @return Copy of the result
		 */
		inline constexpr Vec4		operator/= (const float f) noexcept
		{
			x /= f;
			y /= f;
			z /= f;
			w /= f;
			return *this;
		}
	
		/**
		 * Divides current templated vector by given vector
		 * @param v: Templated vector to divide by
		 * @return Copy of the result
		 */
		inline constexpr Vec4		operator/= (const Vec<4>& v) noexcept
		{
			x /= v[0];
			y /= v[1];
			z /= v[2];
			w /= v[3];
			return *this;
		}
	
		/**
		 * Divides current vector by given vector
		 * @param v: Vector to divide by
		 * @return Copy of the result
		 */
		inline constexpr Vec4		operator/= (const Vec4& v) noexcept
		{
			x /= v.x;
			y /= v.y;
			z /= v.z;
			w /= v.w;
			return *this;
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
			m.Set(3, w);

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
			m.Set(3, w);

			return m;
		}

		/**
		 * Converts vector to string format
		 * @return String version of the current vector
		 */
		inline std::string			ToString() const noexcept
		{
			return std::to_string(x) + std::to_string(y) + std::to_string(z) + std::to_string(w);
		}
	};
}

#endif
