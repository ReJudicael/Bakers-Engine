#ifndef __VEC__
#define __VEC__

#include "MathMinimal.h"

#include "Mat.hpp"
#include <iostream>

namespace Core::Maths
{
	/**
	 * Templated size vector class
	 * @tparam Size: Size of the vector
	 */
	template <int Size>
	struct Vec
	{
		float			m_vec[Size];

		/**
		 * Default constructor, creates a vec zero
		 */
		inline constexpr		Vec() noexcept : m_vec()
		{
			for (unsigned i {0}; i < Size; ++i)
				m_vec[i] = 0;
		}

		/**
		 * Creates a vector with its components initialized at the 
		 * value of the array v
		 * @param v: Array initializer of the vector
		 */
		inline explicit constexpr		Vec(const float v[Size]) noexcept : m_vec()
		{
			for (unsigned i {0}; i < Size; ++i)
				m_vec[i] = v[i];
		}
		
		/**
		 * Creates a vector with all of its components initialized
		 * at the value of f
		 * @param f: Initializer value
		 */
		inline explicit constexpr		Vec(const float f) noexcept : m_vec()
		{
			for (unsigned i {0}; i < Size; ++i)
				m_vec[i] = f;
		}

		/**
		 * Creates a vector from a column matrix
		 * @param m: Column matrix initializer
		 */
		inline constexpr		Vec(const Mat<Size, 1> & m) noexcept : m_vec()
		{
			for (unsigned i{ 0 }; i < Size; ++i)
				m_vec[i] = m(i);
		}

		/**
		 * Creates a copy of the given vector
		 * @param v: Vector to be copied
		 */
		inline constexpr		Vec(const Vec<Size>& v) noexcept : m_vec()
		{
			for (unsigned i {0}; i < Size; ++i)
				m_vec[i] = v[i];
		}

		/**
		 * Moves a vector to a new vector
		 * @param v: Vector to be moved
		 */
		inline constexpr		Vec(Vec<Size>&& v) noexcept : m_vec{}
		{
			for (unsigned i{ 0 }; i < Size; ++i)
				m_vec[i] = std::move(v[i]);
		}

		/**
		 * Computes normalized version of current vector
		 * @return Normalized version of current vector
		 */
		inline constexpr Vec<Size>	Normalized() const noexcept
		{
			return *this / Length(); 
		}		

		/**
		 * Normalizes current vector
		 */
		inline constexpr void		Normalize() noexcept
		{
			*this /= Length();
		}
			
		/**
		 * Computes length of current vector
		 * @return Length of current vector
		 */
		inline constexpr float		Length() const noexcept
		{
			return sqrt(SquaredLength());
		}

		/**
		 * Computes squared length of current vector
		 * @return Squared length of current vector
		 */
		inline constexpr float		SquaredLength() const noexcept
		{
			float	sqrLen {0};
			for (unsigned i {0}; i < Size; ++i)
				sqrLen += m_vec[i] * m_vec[i];
			return sqrLen;
		}

		/**
		 * Computes dot product of current vector and
		 * given vector
		 * @param v: Vector to dot product with
		 * @return Result of the dot product
		 */
		inline constexpr float		Dot(const Vec<Size>& v) const noexcept
		{
			float	f {0};
			for (unsigned i {0}; i < Size; ++i)
				f += m_vec[i] * v[i];
			
			return f;
		}

		/**
		 * Computes cross product of current vector and
		 * given vector
		 * @param v: Vector to cross product with
		 * @return Result of the cross product
		 */
		inline constexpr Vec<Size>	Cross(const Vec<Size>& v) const noexcept
		{
			Vec<Size>	newVec {};
			for (unsigned i {0}; i < Size; ++i)
			{
				if (i + 2 == Size)
					newVec[i] = m_vec[0] * v[1] - m_vec[1] * v[0];
				else if (i + 3 == Size)
					newVec[i] = m_vec[i + 1] * v[0] - m_vec[0] * v[i + 1];
				else
					newVec[i] = m_vec[i + 1] * v[i + 2] - m_vec[i + 2] * v[i + 1];
			}
			return newVec;
		}

		/**
		 * Setter that takes as parameter position in the vector
		 * @param i: Position in the vector
		 * @param f: Value to be set
		 */
		inline constexpr void		Set(const unsigned i, const float f) noexcept
		{
			m_vec[i] = f;
		}

		/**
		 * Settet that moves the value in the vector and
		 * takes as parameter the position in the vector
		 * @param i: Position in the vector
		 * @param f: Value to be moved
		 */
		inline constexpr void		Set(const unsigned i, float&& f) noexcept
		{
			m_vec[i] = std::move(f);
		}

		/**
		 * Getter that takes as parameter the position in the vector
		 * and returns a reference to the required component
		 * @param i: Position in the vector
		 * @return Reference to required component of the vector
		 */
		inline constexpr float&		operator[] (const unsigned i) noexcept
		{
			return m_vec[i];
		}

		/**
		 * Getter that takes as parameter the position in the vector
		 * and returns a copy to the required component
		 * @param i: Position in the vector
		 * @return Copy to required component of the vector
		 */
		inline constexpr float		operator[] (const unsigned i) const noexcept
		{
			return m_vec[i];
		}

		/**
		 * Copies given vector in the current vector
		 * @param v: Vector to be copied
		 * @return Copy of the result
		 */
		inline constexpr Vec<Size>	operator= (const Vec<Size>& v) noexcept
		{
			for (unsigned i {0}; i < Size; ++i)
				m_vec[i] = v[i];

			return *this;
		}

		/**
		 * Moves the given vector in the current vector
		 * @param v: Vector to be moved
		 * @return Copy of the result
		 */
		inline constexpr Vec<Size>	operator= (Vec<Size>&& v) noexcept
		{
			for (unsigned i {0}; i < Size; ++i)
				m_vec[i] = std::move(v[i]);
			return *this;
		}
	
		/**
		 * Compares the given vector to the current vector
		 * @param v: Vector to compare to
		 * @return The result of the comparison
		 */
		inline constexpr bool		operator== (const Vec<Size>& v) const noexcept
		{
			for (unsigned i {0}; i < Size; ++i)
				if (v[i] != m_vec[i])
					return false;
			return true;
		}

		/**
		 * Compares the given vector to the current vector
		 * @param v: Vector to compare to
		 * @return The result of the comparison
		 */
		inline constexpr bool		operator!= (const Vec<Size>& v) const noexcept
		{
			for (unsigned i {0}; i < Size; ++i)
				if (v[i] != m_vec[i])
					return true;
			return false;
		}
		
		/**
		 * Computes the addition of the current vector to
		 * the given vector
		 * @param v: Vector to add
		 * @return The result of the addition
		 */
		inline constexpr Vec<Size>	operator+ (const Vec<Size>& v) const noexcept
		{
			Vec<Size>	newVec {v};
			for (unsigned i {0}; i < Size; ++i)
				newVec[i] += m_vec[i];
			return newVec;
		}

		/**
		 * Computes the substaction of the given vector
		 * to the current vector
		 * @param v: Vector to substract
		 * @return The result of the substaction
		 */
		inline constexpr Vec<Size>	operator- (const Vec<Size>& v) const noexcept
		{
			Vec<Size>	newVec {*this};
			for (unsigned i {0}; i < Size; ++i)
				newVec[i] -= v[i];
			return newVec;
		}

		/**
		 * Computes the multiplication of the components
		 * of the current vector by the respective components
		 * of the given vector
		 * @param v: Vector to myltiply by
		 * @return The result of the multiplication
		 */
		inline constexpr Vec<Size>	operator* (const Vec<Size>& v) const noexcept
		{
			Vec<Size>	newVec {v};
			for (unsigned i {0}; i < Size; ++i)
				newVec[i] *= m_vec[i];
			return newVec;
		}

		/**
		 * Computes the division of the components
		 * of the current vector by the respective components
		 * of the given vector
		 * @param v: Vector to diviside by
		 * @return The result of the division
		 */
		inline constexpr Vec<Size>	operator/ (const Vec<Size>& v) const noexcept
		{
			Vec<Size>	newVec {*this};
			for (unsigned i {0}; i < Size; ++i)
				newVec[i] /= v[i];
			return newVec;
		}

		/**
		 * Computes the division of the components
		 * of the current vector by the given scalar
		 * @param f: Scalar to divide by
		 * @return The result of the scalar division
		 */
		inline constexpr Vec<Size>	operator/ (float f) const noexcept
		{
			Vec<Size>	newVec{ *this };
			for (unsigned i{ 0 }; i < Size; ++i)
				newVec[i] /= f;
			return newVec;
		}

		/**
		 * Adds given vector to current vector
		 * @param v: Vector to add
		 * @return Copy of the result
		 */
		inline constexpr Vec<Size>	operator+= (const Vec<Size>& v) noexcept
		{
			for (unsigned i {0}; i < Size; ++i)
				m_vec[i] += v[i];
			return *this;
		}

		/**
		 * Substracts given vector to current vector
		 * @param v: Vector to substract
		 * @return Copy of the result
		 */
		inline constexpr Vec<Size>	operator-= (const Vec<Size>& v) noexcept
		{
			for (unsigned i {0}; i < Size; ++i)
				m_vec[i] -= v[i];
			return *this;
		}

		/**
		 * Multiply given vector to current vector
		 * @param v: Vector to multiply
		 * @return Copy of the result
		 */
		inline constexpr Vec<Size>	operator*= (const Vec<Size>& v) noexcept
		{
			for (unsigned i {0}; i < Size; ++i)
				m_vec[i] *= v[i];
			return *this;
		}
	
		/**
		 * Divides current vector by given vector
		 * @param v: Vector to divide by
		 * @return Copy of the result
		 */
		inline constexpr Vec<Size>	operator/= (const Vec<Size>& v) noexcept
		{
			for (unsigned i {0}; i < Size; ++i)
				m_vec[i] /= v[i];
			return *this;
		}

		/**
		 * Divides current vector by given scalar
		 * @param f: Scalar to divide by
		 * @return Copy of the result
		 */
		inline constexpr Vec<Size>	operator/= (float f) noexcept
		{
			for (unsigned i{ 0 }; i < Size; ++i)
				m_vec[i] /= f;
			return *this;
		}
		
		/**
		 * Converts vector into column matrix
		 */
		inline constexpr			operator Mat<Size, 1>() const noexcept
		{
			Mat<Size, 1> m;
			for (unsigned i{ 0 }; i < Size; ++i)
				m.Set(i, m_vec[i]);

			return m;
		}

		/**
		 * Converts vector into line matrix
		 */
		inline constexpr			operator Mat<1, Size>() const noexcept
		{
			Mat<1, Size> m;
			for (unsigned i{ 0 }; i < Size; ++i)
				m.Set(i, m_vec[i]);

			return m;
		}

		/**
		 * Converts vector to string format
		 * @return String version of the current vector
		 */
		inline std::string			ToString() const noexcept
		{
			std::string s;
			for (unsigned i{ 0 }; i < Size; ++i)
			{
				s += std::to_string(m_vec[i]) + " ";
			}
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
}

#endif
