#ifndef __REF3D__
#define __REF3D__

#include "MathMinimal.h"

#include "Vec3.hpp"
#include "Mat4.hpp"
#include "Ref.hpp"

namespace Core::Maths
{
	/**
	 * Referential structure made for 3D referentials. Computes world to local
	 * and local to world matrix from the referential
	 */
	struct Ref3D
	{
		Vec3	origin;
		Vec3	i;
		Vec3	j;
		Vec3	k;
		
		/**
		 * Creates a default referential, starting at the origin and with default axis
		 */
		inline constexpr	Ref3D() noexcept : origin {0, 0, 0}, i {1, 0, 0}, j {0, 1, 0}, k {0, 0, 1}
		{
		}

		/**
		 * Creates a referential with set origin and axis
		 * @param o: Origin of the ref
		 * @param I: X axis of the referential
		 * @param J: Y axis of the referential
		 * @param K: Z axis of the referential
		 */
		inline constexpr	Ref3D(const Vec3& o, const Vec3& I, const Vec3& J, const Vec3& K) noexcept
			: origin {o}, i {I}, j {J}, k {K}
		{
		}

		/**
		 * Copy constructor
		 * @param r: Ref to copy
		 */
		inline constexpr	Ref3D(const Ref3D& r) noexcept
			: origin{ r.origin }, i{ r.i }, j{ r.j }, k{ r.k }
		{
		}

		/**
		 * Copy constructor from templated ref
		 * @param r: Templated ref to copy
		 */
		inline constexpr	Ref3D(const Ref<3> & r) noexcept
			: origin{r.origin}, i {r[0]}, j {r[1]}, k {r[2]}
		{
		}
		
		/**
		 * Computes the world version of the given vector from this ref
		 * @param v: Vector to compute world version
		 * @return World version of the vector
		 */
		inline constexpr Vec3	LocalToWorld(const Vec3& v) const noexcept
		{
			return LocalToWorldMatrix() * (Mat<4, 1>)v;
		}

		/**
		 * Computes local coordinates of the given world vector
		 * @param v: World vector to compute local version
		 * @result Local version of given vector
		 */
		inline constexpr Vec3	WorldToLocal(const Vec3& v) const noexcept
		{
			return WorldToLocalMatrix() * (Mat<4, 1>)v;
		}

		/**
		 * Computes the world version of the given vector from this ref
		 * @param v: Vector to compute world version
		 * @return World version of the vector
		 */
		inline constexpr Vec4	LocalToWorld(const Vec4& v) const noexcept
		{
			return LocalToWorldMatrix() * (Mat<4, 1>)v;
		}

		/**
		 * Computes local coordinates of the given world vector
		 * @param v: World vector to compute local version
		 * @return Local version of given vector
		 */
		inline constexpr Vec4	WorldToLocal(const Vec4& v) const noexcept
		{
			return WorldToLocalMatrix() * (Mat<4, 1>)v;
		}

		/**
		 * Computes the world to local matrix of the ref
		 * @return World to local matrix of this ref
		 */
		inline constexpr Mat4	WorldToLocalMatrix() const noexcept
		{
			return LocalToWorldMatrix().Inversed();
		}

		/**
		 * Computes the local to world matrix of the ref
		 * @return Local to world matrix of this ref
		 */
		inline constexpr Mat4	LocalToWorldMatrix() const noexcept
		{
			Mat4	m;

			m.Set(0, 0, i.x);
			m.Set(0, 1, j.x);
			m.Set(0, 2, k.x);
			m.Set(0, 3, origin.x);
			m.Set(1, 0, i.y);
			m.Set(1, 1, j.y);
			m.Set(1, 2, k.y);
			m.Set(1, 3, origin.y);
			m.Set(2, 0, i.z);
			m.Set(2, 1, j.z);
			m.Set(2, 2, k.z);
			m.Set(2, 3, origin.z);
			m.Set(3, 3, 1);

			return m;
		}
		
		/**
		 * Conversion to a templated version of the ref
		 */
		inline constexpr		operator Ref<3>() const noexcept
		{
			Ref<3>	r;
			r.origin = origin;
			r.Set(0, i);
			r.Set(1, j);
			r.Set(2, k);
			return r;
		}
	};
}


#endif
