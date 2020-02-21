#ifndef __QUATERNION__
#define __QUATERNION__

#include <functional>

#include "Vec3.hpp"
#include "Vec4.hpp"
#include "Mat4.hpp"

namespace Core::Maths
{
	/**
	 * A quaternion struct to make rotations
	 */
	struct Quaternion
	{
		union
		{
			struct
			{
				float	w;
				float	x;
				float	y;
				float	z;
			};
			struct
			{
				Vec<2>	wx;
				Vec<2>	yz;
			};
			struct
			{
				float	w;
				Vec3	xyz;
			};
			struct
			{
				Vec3	wxy;
				float	z;
			};
			float		m_vec[4];
		};

		/**
		 * Default constructor, creating identity quaternion
		 */
		inline constexpr		Quaternion() noexcept : w{ 1 }, x{ 0 }, y{ 0 }, z{ 0 }
		{}

		/**
		 * Copy constructor
		 * @param q: Quaternion to be copied
		 */
		inline constexpr		Quaternion(const Quaternion& q) noexcept : w{ q.w }, x{ q.x }, y{ q.y }, z{ q.z }
		{}

		/**
		 * Copy operator
		 * @param q: Quaternion to be copied
		 * @return Reference to current quaternion
		 */
		inline constexpr Quaternion& operator= (const Quaternion& q) noexcept
		{
			w = q.w;
			x = q.x;
			y = q.y;
			z = q.z;
			return *this;
		}

		/**
		 * Move constructor
		 * @param q: Quaternion to be moved
		 */
		inline constexpr		Quaternion(Quaternion&& q) noexcept 
			: w{ std::move(q.w) }, x{ std::move(q.x) }, y{ std::move(q.y) }, z{ std::move(q.z) }
		{}

		/**
		 * Constructor that takes the components of the quaternion as parameters
		 * @param newW: W component of the quaternion
		 * @param newX: X component of the quaternion
		 * @param newY: Y component of the quaternion
		 * @param newZ: Z component of the quaternion
		 */
		inline constexpr		Quaternion(float newW, float newX, float newY, float newZ) noexcept
			: w{ newW }, x{ newX }, y{ newY }, z{ newZ }
		{}

		/**
		 * Constructor that takes an angle and an axis as parameters
		 * @param newW: Angle of the quaternion
		 * @param vec: Axis of the quaternion
		 */
		inline constexpr		Quaternion(float newW, const Vec3& vec) noexcept : w{ newW }, x{ vec.x }, y{ vec.y }, z{ vec.z }
		{}

		/**
		 * Constructor that converts a vec4 to a quaternion. Shifts values around while doing that
		 * @param vec: Vec4 to be converted
		 */
		inline constexpr		Quaternion(const Vec4& vec) noexcept : w{ vec.x }, x{ vec.y }, y{ vec.z }, z{ vec.w }
		{}

		/**
		 * Constructor that creates a quaternion from euler angles
		 * @param eulerX: X rotation of the euler angle
		 * @param eulerY: Y rotation of the euler angle
		 * @param eulerZ: Z rotation of the euler angle
		 */
		inline					Quaternion(float eulerX, float eulerY, float eulerZ) noexcept : Quaternion(0, 0, 0, 0)
		{
			float cx{ cos(eulerX / 2.f) }, cy{ cos(eulerY / 2.f) }, cz{ cos(eulerZ / 2.f) };
			float sx{ sin(eulerX / 2.f) }, sy{ sin(eulerY / 2.f) }, sz{ sin(eulerZ / 2.f) };
			w = cx * cy * cz - sx * sy * sz;
			x = sx * cy * cz - cx * sy * sz;
			y = cx * sy * cz - sx * cy * sz;
			z = cx * cy * sz - sx * sy * cz;
		}

		/**
		 * Constructor that creates a quaternion from euler angles
		 * @param euler: Vec3 containing euler angles
		 */
		inline					Quaternion(const Vec3& euler) noexcept : Quaternion(euler.x, euler.y, euler.z)
		{}

		/**
		 * Fills the quaternion with zeroes
		 */
		inline constexpr void	Zero() noexcept
		{
			w = 0.f;
			x = 0.f;
			y = 0.f;
			z = 0.f;
		}

		/**
		 * Returns a zero quaternion
		 * @return Zero quaternion
		 */
		inline constexpr Quaternion	Zero() const noexcept
		{
			constexpr Quaternion q{0.f, 0.f, 0.f, 0.f};
			return q;
		}

		/**
		 * Returns vec component of the quaternion
		 * @return Vector components of the quaternion
		 */
		inline constexpr Vec3		GetVec() const noexcept
		{
			return Vec3(x, y, z);
		}

		/**
		 * Computes squared length of the quaternion
		 * @return Squared length of the quaternion
		 */
		inline constexpr float	SqrLength() const noexcept
		{
			return w * w + x * x + y * y + z * z;
		}

		/**
		 * Computes length of the quaternion
		 * @return Length of the quaternion
		 */
		inline float	Length() const noexcept
		{
			return sqrt(SqrLength());
		}

		/**
		 * Normalizes current quaternion
		 */
		inline void	Normalize() noexcept
		{
			const float len{ Length() };
			w /= len;
			x /= len;
			y /= len;
			z /= len;
		}

		/**
		 * Computes current quaternion normalized
		 * @return Current quaternion normalized
		 */
		inline Quaternion Normalized() const noexcept
		{
			const float len{ Length() };
			Quaternion q;
			q.w = w / len;
			q.x = x / len;
			q.y = y / len;
			q.z = z / len;

			return q;
		}

		/**
		 * Negates all components of the quaternion
		 */
		inline constexpr void	Negate() noexcept
		{
			w *= -1;
			x *= -1;
			y *= -1;
			z *= -1;
		}

		/**
		 * Computes the difference between current and given quaternion
		 * @param q: Quaternion to compute difference with
		 * @return The difference between the two quaternions
		 */
		inline constexpr Quaternion operator-(const Quaternion& q) const noexcept
		{
			return q * Inversed();
		}

		/**
		 * Set current quaternion to difference between itself and given quaternion
		 * @param q: Quaternion to compute difference with
		 * @return Reference to current quaternion
		 */
		inline constexpr Quaternion& operator-= (const Quaternion& q) noexcept
		{
			*this = q * Inversed();
			return *this;
		} 

		/**
		 * Compute the addition of the current quaternion with the given quaternion.
		 * Only adds the components of the quaternions
		 * @param q: Quaternion to add
		 * @return The addition of the two quaternions
		 */
		inline constexpr Quaternion	operator+ (const Quaternion& q) const noexcept
		{
			return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
		}

		/**
		 * Adds the given quaternion to current quaternion.
		 * Only adds the components of the quaternions
		 * @param q: Quaternion to add
		 * @return Reference to current quaternion
		 */
		inline constexpr Quaternion& operator+= (const Quaternion& q) noexcept
		{
			w += q.w;
			x += q.x;
			y += q.y;
			z += q.z;

			return *this;
		}

		/**
		 * Computes current quaternion multiplied by scalar
		 * @param scalar: Scalar to multiply by
		 * @return Quaternion multiplied by scalar
		 */
		inline constexpr Quaternion	operator* (float scalar) const noexcept
		{
			return Quaternion(scalar * w, scalar * x, scalar * y, scalar * z);
		}

		/**
		 * Multiplies current quaternion by scalar
		 * @param scalar: Scalar to multiply by
		 * @return Reference to quaternion
		 */
		inline constexpr Quaternion& operator*= (float scalar) noexcept
		{
			w *= scalar;
			x *= scalar;
			y *= scalar;
			z *= scalar;

			return *this;
		}

		/**
		 * Computes multiplication between current and given quaternions
		 * @param q: Quaternion to multiply
		 * @return Result of the multiplication
		 */
		inline constexpr Quaternion	operator* (const Quaternion& q) const noexcept
		{
			return Quaternion(
				w * q.w - x * q.x - y * q.y - z * q.z,
				w * q.x + x * q.w + y * q.z - z * q.y,
				w * q.y + y * q.w + z * q.x - x * q.z,
				w * q.z + z * q.w + x * q.y - y * q.x
			);
		}

		/**
		 * Multiplies current quaternion by given quaternion
		 * @param q: Quaternion to multiply
		 * @return Reference to current quaternion
		 */
		inline constexpr Quaternion& operator*= (const Quaternion& q) noexcept
		{
			w = w * q.w - x * q.x - y * q.y - z * q.z;
			x = w * q.x + x * q.w + y * q.z - z * q.y;
			y = w * q.y + y * q.w + z * q.x - x * q.z;
			z = w * q.z + z * q.w + x * q.y - y * q.x;

			return *this;
		}
		
		/**
		 * Computes quaternion division by scalar
		 * @param scalar: Scalar to divide by
		 * @return Result of the division
		 */
		inline constexpr Quaternion	operator/ (const float scalar) const noexcept
		{
			return Quaternion(w / scalar, x / scalar, y / scalar, z / scalar);
		}

		/**
		 * Divides current quaternion by scalar
		 * @param scalar: Scalar to divide by
		 * @return Reference to current quaternion
		 */
		inline constexpr Quaternion& operator/= (const float scalar) noexcept
		{
			w /= scalar;
			x /= scalar;
			y /= scalar;
			z /= scalar;

			return *this;
		}

		/**
		 * Computes dot product of the two quaternions. Works just like vector dot product
		 * @param q: Quaternion to dot with
		 * @return Result of the dot product
		 */
		inline constexpr float		Dot(const Quaternion& q) const noexcept
		{
			return w * q.w + x * q.x + y * q.y + z * q.z;
		}

		/**
		 * Computes slerp of current quaternion with given quaternion, with given progress
		 * @param q: Quaternion to slerp with
		 * @param progress: Progress of the slerp
		 * @result The result of the slerp
		 */
		inline Quaternion			Slerp(Quaternion q, float progress) const noexcept
		{
			float dot{ Dot(q) };
			if (dot < 0.0f)
			{
				q.Negate();
				dot = -dot;
			}

			constexpr float DOT_THRESHOLD{ 0.9995f };
			if (dot > DOT_THRESHOLD)
			{
				Quaternion	result{ *this + (q - *this) * progress };
			}

			float theta {acos(dot)};

			return Quaternion((*this * sin((1 - progress) * theta) + q * sin(progress * theta)) / sin(theta));
		}

		/**
		 * Computes the conjugated of current quaternion
		 * @return Conjugated of current quaternion
		 */
		inline constexpr Quaternion	Conjugated() const noexcept
		{
			return Quaternion(w, -x, -y, -z);
		}

		/**
		 * Computes the inverse of the quaternion
		 * @return Inverse of the quaternion
		 */
		inline constexpr Quaternion	Inversed() const noexcept
		{
			return Conjugated() / SqrLength();
		}

		/**
		 * Computes the inverse of the quaternion. It must be a unit quaternion,
		 * meaning it has to have a length of one
		 * @return Inverse of the quaternion
		 */
		inline constexpr Quaternion	InversedUnit() const noexcept
		{
			return Conjugated();
		}

		/**
		 * Returns the angle of the quaternion. It must be a unit quaternion,
		 * meaning it has to have a length of one
		 * @return Angle of the quaternion
		 */
		inline float				GetAngleUnit() const noexcept
		{
			return 2 * acos(w);
		}

		/**
		 * Computes the rotation matrix of the quaternion
		 * @return Rotation matrix of the quaternion
		 */
		inline constexpr Mat4		GetMat() const noexcept
		{
			return GetMatUnit() / SqrLength();
		}

		/**
		 * Computes the rotation matrix of the quaternion. It must be a unit quaternion,
		 * meaning it has to have a length of one
		 * @return Rotation matrix of the quaternion
		 */
		inline constexpr Mat4		GetMatUnit() const noexcept
		{
			Mat4	mat{};
			
			mat.Set(0, 0, 1.f - 2.f * (y * y + z * z));
			mat.Set(0, 1, 2.f * (x * y - z * w));
			mat.Set(0, 2, 2.f * (x * z + w * y));
			mat.Set(1, 0, 2.f * (x * y + w * z));
			mat.Set(1, 1, 1.f - 2.f * (x * x + z * z));
			mat.Set(1, 2, 2.f * (y * z - w * x));
			mat.Set(2, 0, 2.f * (x * z - w * y));
			mat.Set(2, 1, 2.f * (y * z + w * x));
			mat.Set(2, 2, 1.f - 2.f * (x * x + y * y));
			mat.Set(3, 3, 1.f);

			return mat;
		}

		/**
		 * Returns string version of the quaternion
		 * @return String version of the quaternion
		 */
		inline std::string			ToString() const noexcept
		{
			std::string result{ "w: " };
			result += std::to_string(w);
			result += std::string(", x: ");
			result += std::to_string(x);
			result += std::string(", y: ");
			result += std::to_string(y);
			result += std::string(", z: ");
			result += std::to_string(z);
			return result;
		}
		/**
		 * Prints the quaternion
		 */
		inline void					Print() const noexcept
		{
			std::cout << ToString() << std::endl;
		}
	};

	/**
	 * Scalar multiplication of float and quaternion
	 * @param f: Scalar to multiply by
	 * @param q: Quaternion to multiply
	 * @return Result of the multiplication
	 */
	inline constexpr Quaternion	operator* (float f, const Quaternion& q) noexcept
	{
		return q * f;
	}

	/**
	 * Negation operator
	 * @param q: Quaternion to negate
	 * @return The negated version of the quaternion
	 */
	inline constexpr Quaternion	operator- (Quaternion q) noexcept
	{
		q.Negate();
		return q;
	}

	/**
	 * Division of the scalar f by the components of q
	 * @param f: Scalar to be divided
	 * @param q: Quaternion to divide by
	 * @return Quaternion composed of f divided by the components of q
	 */
	inline constexpr Quaternion	operator/ (float f, Quaternion q) noexcept
	{
		q.w = f / q.w;
		q.x = f / q.x;
		q.y = f / q.y;
		q.z = f / q.z;

		return q;
	}

	/**
	 * Typedef of Quaternion to reduce number of characters
	 */
	using Quat = Quaternion;
}

#endif
