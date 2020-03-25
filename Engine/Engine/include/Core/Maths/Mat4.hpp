#ifndef __MAT4__
#define __MAT4__

#include "MathMinimal.h"

#include "Mat.hpp"
#include "Vec4.hpp"

namespace Core::Maths
{
	/**
	 * Matrix class with predefined size of 4 by 4, 
	 * for a total size of 16
	 */
	struct Mat4
	{
		union
		{
			float		m_array[16];
			float	    m_mat[4][4];
			Mat<4, 4>	mat;
		};

		/**
		 * Generates matrix zero
		 */
		inline constexpr			Mat4() noexcept : m_array() {};

		/**
		 * Creates a copy of given matrix
		 * @param m: Matrix to be copied
		 */
		inline constexpr			Mat4(const Mat4& m) noexcept;

		/**
		 * Creates a copy of given templated matrix
		 * @param m: Templated matrix to be copied
		 */
		inline constexpr			Mat4(const Mat<4, 4>& m) noexcept;

		/**
		 * Moves given matrix to new matrix
		 * @param m: Matrix to be moved
		 */
		inline constexpr			Mat4(const Mat4&& m) noexcept;

		/**
		 * Moves given templated matrix to new matrix
		 * @param m: Templated matrix to be moved
		 */
		inline constexpr			Mat4(const Mat<4, 4>&& m) noexcept;

		/**
		 * Creates a matrix from an array of floats of size 16
		 * @param m: Array of float to be copied
		 */
		inline						Mat4(const float m[16]) noexcept;

		/**
		 * Copies a templated matrix into current matrix
		 * @param m: Templated matrix to be copied
		 */
		inline constexpr Mat4		operator= (const Mat<4, 4> & m) noexcept;
		/**
		 * Copies a matrix into current matrix
		 * @param m: Matrix to be copied
		 */
		inline constexpr Mat4		operator= (const Mat4 & m) noexcept;


		// Getters and setters
		/**
		 * Getter that takes as argument only the
		 * position in the array of the desired value
		 * @param a: Position in the array
		 * @return Reference to value requested
		 */
		inline constexpr float		Get(const unsigned a) const noexcept;
		/**
		 * Getter that takes the requested position
		 * in the array as parameter
		 * @param a: Position in the array
		 * @return Reference to value requested
		 */
		inline constexpr float&		Get(const unsigned a) noexcept;
		/**
		 * Getter that takes the requested position
		 * in the matrix as parameter
		 * @param line: Line of the requested value
		 * @param column: Column of the requested value
		 * @return Value requested
		 */
		inline constexpr float		Get(const unsigned line, const unsigned column) const noexcept;
		/**
		 * Getter that takes the requested position
		 * in the matrix as template parameter
		 * @tparam line: Line of the requested value
		 * @tparam column: Column of the requested value
		 * @return Value requested
		 */
		template <unsigned line, unsigned column>
		inline constexpr float		Get() const noexcept;
		/**
		 * Getter that takes the requested position
		 * in the matrix as parameter
		 * @param line: Line of the requested value
		 * @param column: Column of the requested value
		 * @return Reference to value requested
		 */
		inline constexpr float&		Get(const unsigned line, const unsigned column) noexcept;
		/**
		 * Getter that takes the requested position
		 * in the matrix as template parameter
		 * @tparam line: Line of the requested value
		 * @tparam column: Column of the requested value
		 * @return Reference to value requested
		 */
		template <unsigned line, unsigned column>
		inline constexpr float&		Get() noexcept;

		/**
		 * Setter that takes the requested position
		 * in the array as parameter
		 * @param a: Position of the value to be set
		 * @param f: Value to be set
		 */
		inline constexpr void		Set(const unsigned a, const float f) noexcept;
		/**
		 * Setter that takes the requested position
		 * in the matrix as parameter
		 * @param line: Line of the value to be set
		 * @param column: Column of the value to be set
		 * @param f: Value to be set
		 */
		inline constexpr void		Set(const unsigned line, const unsigned column, const float f) noexcept;
		/**
		 * Setter that takes the requested position
		 * in the matrix as template parameter
		 * @tparam line: Line of the value to be set
		 * @tparam column: Column of the value to be set
		 * @param f: Value to be set
		 */
		template <unsigned line, unsigned column>
		inline constexpr void		Set(float f) noexcept;
		/**
		 * Getter that takes the requested position
		 * in the array as parameter.
		 * @param a: Position of the requested value
		 * @result Value requested
		 */
		inline constexpr float		operator() (const unsigned a) const noexcept;
		/**
		 * Getter that takes the requested position
		 * in the array as parameter.
		 * @param a: Position of the requested value
		 * @result Reference to value requested
		 */
		inline constexpr float&		operator() (const unsigned a) noexcept;
		/**
		 * Getter that takes the requested position
		 * in the matrix as parameter.
		 * @param line: Line of the requested value
		 * @param column: Column of the requested value
		 * @result Value requested
		 */
		inline constexpr float		operator() (const unsigned line, const unsigned column) const noexcept;
		/**
		 *Getter that takes the requested position
		 * in the matrix as parameter.
		 * @param line: Line of the requested value
		 * @param column: Column of the requested value
		 * @result Reference to value requested
		 */
		inline constexpr float&		operator() (const unsigned line, const unsigned column) noexcept;

		// Operators
		// Scalars
		/**
		 * Scalar division of the current matrix by a value
		 * @param f: Scalar to divide by
		 * @return Current matrix divided by scalar f
		 */
		inline constexpr Mat4	operator/ (const float f) const noexcept;
		/**
		 * Scalar division of the current matrix by a value
		 * @param f: Scalar to divide by
		 * @return Reference to current matrix
		 */
		inline constexpr Mat4&	operator/= (const float f) noexcept;
		/**
		 * Scalar multiplication of the current matrix by a value
		 * @param f: Scalar to multiply
		 * @return Current matrix multiplied by scalar f
		 */
		inline constexpr Mat4	operator* (const float f) const noexcept;
		/**
		 * Scalar multiplication of the current matrix by a value
		 * @param f: Scalar to multiply
		 * @return Reference to current matrix
		 */
		inline constexpr Mat4&	operator*= (const float f) noexcept;


		// mat4
		/**
		 * Adds the values of the two matrices
		 * @param m: Matrix to add
		 * @return Matrix that represents the addition of the two matrices
		 */
		inline constexpr Mat4	operator+ (const Mat4& m) const noexcept;
		/**
		 * Adds the value of the given matrix to
		 * the value of the current matrix
		 * @param m: The matrix to add
		 * @return Result of the addition
		 */
		inline constexpr Mat4&	operator+= (const Mat4& m) noexcept;
		/**
		 * Substracts the values of the two matrices
		 * @param m: Matrix to substract
		 * @return Matrix that represents the Substraction of the two matrices
		 */
		inline constexpr Mat4	operator- (const Mat4& m) const noexcept;
		/**
		 * Substracts the value of the given matrix
		 * to the value of the current matrix
		 * @param m: Matrix to substract
		 * @return Matrix that represents the Substraction of the two matrices
		 */
		inline constexpr Mat4&	operator-= (const Mat4& m) noexcept;
		/**
		 * Multiplies the values of the two matrices
		 * @param m: Matrix to multiply
		 * @return Matrix that represents the multiplication of the twi matrices
		 */
		inline constexpr Mat4	operator* (const Mat4& m) const noexcept;
		/**
		 * Multiplies the value of the given matrix
		 * to the value of the current matrix
		 * @param m: Matrix to multiply
		 * @return Matrix that represents the multiplication of the twi matrices
		 */
		inline constexpr Mat4&	operator*= (const Mat4& m) noexcept;
		/**
		 * Multiplies the values of the two matrices
		 * @param m: Matrix to multiply
		 * @return Matrix that represents the multiplication of the twi matrices
		 */
		inline constexpr Mat4	operator* (const Mat<4, 4>& m) const noexcept;
		/**
		 * Multiplies the value of the given matrix
		 * with the value of the current matrix
		 * @param m: Matrix to multiply
		 * @return Matrix that represents the multiplication of the twi matrices
		 */
		inline constexpr Mat4&	operator*= (const Mat<4, 4>& m) noexcept;
		/**
		 * Multiplies the values of the current matrix with
		 * a given vector
		 * @param v: Vector to multiply
		 * @return Matrix that represents the multiplication
		 */
		inline constexpr Vec<4>	operator* (const Vec<4>& v) const noexcept;
		/**
		 * Multiplies the values of the current matrix with
		 * a given vector
		 * @param v: Vector to multiply
		 * @return Matrix that represents the multiplication
		 */
		inline constexpr Vec4	operator* (const Vec4& v) const noexcept;
		/**
		 * Multiplies the value of the given matrix
		 * with the value of the current matrix
		 * @param m: Templated matrix to multiply
		 * @return Matrix that represents the multiplication of the twi matrices
		 */
		template <unsigned Line, unsigned Column, typename = std::enable_if_t<Line == 4>>
		inline Mat<4, Column>	operator* (const Mat<Line, Column>& m) const noexcept;
		
		/**
		 * Inverses current matrix
		 */
		inline constexpr void				Inverse();
		/**
		 * Computes the inverse of the current matrix
		 * @return Inverse of current matrix
		 */
		inline constexpr Mat4				Inversed() const noexcept;

		/**
		 * Computes the determinant of the current matrix
		 * @return Determinant of current matrix
		 */
		inline constexpr float				Det() const noexcept;

		/**
		 * Returns the matrix in a string format, with a tabulation
		 * between each number and end line at the end of each line
		 * @return The string representation of the current matrix
		 */
		inline std::string					ToString() const noexcept;
		/**
		 * Print current matrix to the console
		 */
		inline void							Print() const noexcept;
	};

	constexpr Core::Maths::Mat4::Mat4(const Mat4& m) noexcept : m_array()
	{
		for (unsigned i{ 0 }; i < 16; ++i)
			m_array[i] = m.m_array[i];
	}
	
	constexpr Core::Maths::Mat4::Mat4(const Mat<4, 4> & m) noexcept : m_array()
	{
		for (unsigned i{ 0 }; i < 16; ++i)
			m_array[i] = m.m_mat[i];
	}

	constexpr Mat4 Maths::Mat4::operator=(const Mat<4, 4>& m) noexcept
	{
		for (unsigned i{ 0 }; i < 16; ++i)
			m_array[i] = m.m_mat[i];
		return *this;
	}

	constexpr Mat4 Maths::Mat4::operator=(const Mat4& m) noexcept
	{
		for (unsigned i{ 0 }; i < 16; ++i)
			m_array[i] = m.m_array[i];
		return *this;
	}

	constexpr Mat4::Mat4(const Mat4&& m) noexcept : m_array()
	{
		for (unsigned i{ 0 }; i < 16; ++i)
			m_array[i] = std::move(m.m_array[i]);
	}

	constexpr Mat4::Mat4(const Mat<4, 4>&& m) noexcept : m_array()
	{
		for (unsigned i{ 0 }; i < 16; ++i)
			m_array[i] = std::move(m.m_mat[i]);
	}

	Mat4::Mat4(const float m[16]) noexcept  : m_array()
	{
		for (unsigned i{ 0 }; i < 16; ++i)
			m_array[i] = m[i];
	}

	inline constexpr float Maths::Mat4::Get(const unsigned a) const noexcept
	{
		return m_array[a];
	}

	inline constexpr float& Maths::Mat4::Get(const unsigned a) noexcept
	{
		return m_array[a];
	}

	inline constexpr float Maths::Mat4::Get(const unsigned line, const unsigned column) const noexcept
	{
		return m_array[line * 4 + column];
	}

	inline constexpr float& Maths::Mat4::Get(const unsigned line, const unsigned column) noexcept
	{
		return m_array[line * 4 + column];
	}

	inline constexpr void Maths::Mat4::Set(const unsigned line, const unsigned column, const float f) noexcept
	{
		m_array[line * 4 + column] = f;
	}

	inline constexpr void Maths::Mat4::Set(const unsigned a, const float f) noexcept
	{
		m_array[a] = f;
	}

	inline constexpr float Maths::Mat4::operator()(const unsigned a) const noexcept
	{
		return m_array[a];
	}

	inline constexpr float& Maths::Mat4::operator()(const unsigned a) noexcept
	{
		return m_array[a];
	}

	inline constexpr float Maths::Mat4::operator()(const unsigned line, const unsigned column) const noexcept
	{
		return m_array[line * 4 + column];
	}

	inline constexpr float& Maths::Mat4::operator()(const unsigned line, const unsigned column) noexcept
	{
		return m_array[line * 4 + column];
	}

	template<unsigned line, unsigned column>
	inline constexpr float Mat4::Get() const noexcept
	{
		constexpr unsigned pos{ line * 4 + column };
		return m_array[pos];
	}

	template<unsigned line, unsigned column>
	inline constexpr float& Mat4::Get() noexcept
	{
		constexpr unsigned pos{ line * 4 + column };
		return m_array[pos];
	}

	template<unsigned line, unsigned column>
	constexpr void Mat4::Set(float f) noexcept
	{
		m_array[line * 4 + column] = f;
	}

	constexpr Mat4 Mat4::operator/(const float f) const noexcept
	{
		Mat4	m{ *this };
		for (unsigned i{ 0 }; i < 16; ++i)
			m.m_array[i] /= f;
		return m;
	}

	constexpr Mat4& Mat4::operator/=(const float f) noexcept
	{
		for (unsigned i{ 0 }; i < 16; ++i)
			m_array[i] /= f;
		return *this;
	}

	constexpr Mat4 Mat4::operator*(const float f) const noexcept
	{
		Mat4	m{ *this };
		for (unsigned i{ 0 }; i < 16; ++i)
			m.m_array[i] *= f;
		return m;
	}

	constexpr Mat4& Mat4::operator*=(const float f) noexcept
	{
		for (unsigned i{ 0 }; i < 16; ++i)
			m_array[i] /= f;
		return *this;
	}

	inline constexpr Mat4 Mat4::operator+(const Mat4& m) const noexcept
	{
		Mat4	result{ *this };
		for (unsigned i{ 0 }; i < 16; ++i)
			result.m_array[i] += m.m_array[i];
		return result;
	}

	inline constexpr Mat4& Mat4::operator+=(const Mat4& m) noexcept
	{
		for (unsigned i{ 0 }; i < 16; ++i)
			m_array[i] += m.m_array[i];
		return *this;
	}
	
	inline constexpr Mat4 Mat4::operator-(const Mat4& m) const noexcept
	{
		Mat4	result{ *this };
		for (unsigned i{ 0 }; i < 16; ++i)
			result.m_array[i] -= m.m_array[i];
		return result;
	}

	inline constexpr Mat4& Mat4::operator-=(const Mat4& m) noexcept
	{
		for (unsigned i{ 0 }; i < 16; ++i)
			m_array[i] -= m.m_array[i];
		return *this;
	}
	constexpr Mat4 Mat4::operator*(const Mat4& m) const noexcept
	{
		Mat4 result{};

		result.Set<0, 0>(Get<0, 0>() * m.Get<0, 0>() +
			Get<0, 1>() * m.Get<1, 0>() +
			Get<0, 2>() * m.Get<2, 0>() +
			Get<0, 3>() * m.Get<3, 0>());
		result.Set<0, 1>(Get<0, 0>() * m.Get<0, 1>() +
			Get<0, 1>() * m.Get<1, 1>() +
			Get<0, 2>() * m.Get<2, 1>() +
			Get<0, 3>() * m.Get<3, 1>());
		result.Set<0, 2>(Get<0, 0>() * m.Get<0, 2>() +
			Get<0, 1>() * m.Get<1, 2>() +
			Get<0, 2>() * m.Get<2, 2>() +
			Get<0, 3>() * m.Get<3, 2>());
		result.Set<0, 3>(Get<0, 0>() * m.Get<0, 3>() +
			Get<0, 1>() * m.Get<1, 3>() +
			Get<0, 2>() * m.Get<2, 3>() +
			Get<0, 3>() * m.Get<3, 3>());
		result.Set<1, 0>(Get<1, 0>() * m.Get<0, 0>() +
			Get<1, 1>() * m.Get<1, 0>() +
			Get<1, 2>() * m.Get<2, 0>() +
			Get<1, 3>() * m.Get<3, 0>());
		result.Set<1, 1>(Get<1, 0>() * m.Get<0, 1>() +
			Get<1, 1>() * m.Get<1, 1>() +
			Get<1, 2>() * m.Get<2, 1>() +
			Get<1, 3>() * m.Get<3, 1>());
		result.Set<1, 2>(Get<1, 0>() * m.Get<0, 2>() +
			Get<1, 1>() * m.Get<1, 2>() +
			Get<1, 2>() * m.Get<2, 2>() +
			Get<1, 3>() * m.Get<3, 2>());
		result.Set<1, 3>(Get<1, 0>() * m.Get<0, 3>() +
			Get<1, 1>() * m.Get<1, 3>() +
			Get<1, 2>() * m.Get<2, 3>() +
			Get<1, 3>() * m.Get<3, 3>());
		result.Set<2, 0>(Get<2, 0>() * m.Get<0, 0>() +
			Get<2, 1>() * m.Get<1, 0>() +
			Get<2, 2>() * m.Get<2, 0>() +
			Get<2, 3>() * m.Get<3, 0>());
		result.Set<2, 1>(Get<2, 0>() * m.Get<0, 1>() +
			Get<2, 1>() * m.Get<1, 1>() +
			Get<2, 2>() * m.Get<2, 1>() +
			Get<2, 3>() * m.Get<3, 1>());
		result.Set<2, 2>(Get<2, 0>() * m.Get<0, 2>() +
			Get<2, 1>() * m.Get<1, 2>() +
			Get<2, 2>() * m.Get<2, 2>() +
			Get<2, 3>() * m.Get<3, 2>());
		result.Set<2, 3>(Get<2, 0>() * m.Get<0, 3>() +
			Get<2, 1>() * m.Get<1, 3>() +
			Get<2, 2>() * m.Get<2, 3>() +
			Get<2, 3>() * m.Get<3, 3>());
		result.Set<3, 0>(Get<3, 0>() * m.Get<0, 0>() +
			Get<3, 1>() * m.Get<1, 0>() +
			Get<3, 2>() * m.Get<2, 0>() +
			Get<3, 3>() * m.Get<3, 0>());
		result.Set<3, 1>(Get<3, 0>() * m.Get<0, 1>() +
			Get<3, 1>() * m.Get<1, 1>() +
			Get<3, 2>() * m.Get<2, 1>() +
			Get<3, 3>() * m.Get<3, 1>());
		result.Set<3, 2>(Get<3, 0>() * m.Get<0, 2>() +
			Get<3, 1>() * m.Get<1, 2>() +
			Get<3, 2>() * m.Get<2, 2>() +
			Get<3, 3>() * m.Get<3, 2>());
		result.Set<3, 3>(Get<3, 0>() * m.Get<0, 3>() +
			Get<3, 1>() * m.Get<1, 3>() +
			Get<3, 2>() * m.Get<2, 3>() +
			Get<3, 3>() * m.Get<3, 3>());

		return result;
	}

	constexpr Mat4& Mat4::operator*=(const Mat4& m) noexcept
	{
		Mat4 result{};
		result.Set<0, 0>(Get<0, 0>() * m.Get<0, 0>() +
			Get<0, 1>() * m.Get<1, 0>() +
			Get<0, 2>() * m.Get<2, 0>() +
			Get<0, 3>() * m.Get<3, 0>());
		result.Set<0, 1>(Get<0, 0>() * m.Get<0, 1>() +
			Get<0, 1>() * m.Get<1, 1>() +
			Get<0, 2>() * m.Get<2, 1>() +
			Get<0, 3>() * m.Get<3, 1>());
		result.Set<0, 2>(Get<0, 0>() * m.Get<0, 2>() +
			Get<0, 1>() * m.Get<1, 2>() +
			Get<0, 2>() * m.Get<2, 2>() +
			Get<0, 3>() * m.Get<3, 2>());
		result.Set<0, 3>(Get<0, 0>() * m.Get<0, 3>() +
			Get<0, 1>() * m.Get<1, 3>() +
			Get<0, 2>() * m.Get<2, 3>() +
			Get<0, 3>() * m.Get<3, 3>());
		result.Set<1, 0>(Get<1, 0>() * m.Get<0, 0>() +
			Get<1, 1>() * m.Get<1, 0>() +
			Get<1, 2>() * m.Get<2, 0>() +
			Get<1, 3>() * m.Get<3, 0>());
		result.Set<1, 1>(Get<1, 0>() * m.Get<0, 1>() +
			Get<1, 1>() * m.Get<1, 1>() +
			Get<1, 2>() * m.Get<2, 1>() +
			Get<1, 3>() * m.Get<3, 1>());
		result.Set<1, 2>(Get<1, 0>() * m.Get<0, 2>() +
			Get<1, 1>() * m.Get<1, 2>() +
			Get<1, 2>() * m.Get<2, 2>() +
			Get<1, 3>() * m.Get<3, 2>());
		result.Set<1, 3>(Get<1, 0>() * m.Get<0, 3>() +
			Get<1, 1>() * m.Get<1, 3>() +
			Get<1, 2>() * m.Get<2, 3>() +
			Get<1, 3>() * m.Get<3, 3>());
		result.Set<2, 0>(Get<2, 0>() * m.Get<0, 0>() +
			Get<2, 1>() * m.Get<1, 0>() +
			Get<2, 2>() * m.Get<2, 0>() +
			Get<2, 3>() * m.Get<3, 0>());
		result.Set<2, 1>(Get<2, 0>() * m.Get<0, 1>() +
			Get<2, 1>() * m.Get<1, 1>() +
			Get<2, 2>() * m.Get<2, 1>() +
			Get<2, 3>() * m.Get<3, 1>());
		result.Set<2, 2>(Get<2, 0>() * m.Get<0, 2>() +
			Get<2, 1>() * m.Get<1, 2>() +
			Get<2, 2>() * m.Get<2, 2>() +
			Get<2, 3>() * m.Get<3, 2>());
		result.Set<2, 3>(Get<2, 0>() * m.Get<0, 3>() +
			Get<2, 1>() * m.Get<1, 3>() +
			Get<2, 2>() * m.Get<2, 3>() +
			Get<2, 3>() * m.Get<3, 3>());
		result.Set<3, 0>(Get<3, 0>() * m.Get<0, 0>() +
			Get<3, 1>() * m.Get<1, 0>() +
			Get<3, 2>() * m.Get<2, 0>() +
			Get<3, 3>() * m.Get<3, 0>());
		result.Set<3, 1>(Get<3, 0>() * m.Get<0, 1>() +
			Get<3, 1>() * m.Get<1, 1>() +
			Get<3, 2>() * m.Get<2, 1>() +
			Get<3, 3>() * m.Get<3, 1>());
		result.Set<3, 2>(Get<3, 0>() * m.Get<0, 2>() +
			Get<3, 1>() * m.Get<1, 2>() +
			Get<3, 2>() * m.Get<2, 2>() +
			Get<3, 3>() * m.Get<3, 2>());
		result.Set<3, 3>(Get<3, 0>() * m.Get<0, 3>() +
			Get<3, 1>() * m.Get<1, 3>() +
			Get<3, 2>() * m.Get<2, 3>() +
			Get<3, 3>() * m.Get<3, 3>());
		*this = result;
		return *this;
	}

	constexpr Mat4 Mat4::operator*(const Mat<4, 4>& m) const noexcept
	{
		Mat4 result{};

		result.Set<0, 0>(Get<0, 0>() * m.Get<0, 0>() +
			Get<0, 1>() * m.Get<1, 0>() +
			Get<0, 2>() * m.Get<2, 0>() +
			Get<0, 3>() * m.Get<3, 0>());
		result.Set<0, 1>(Get<0, 0>() * m.Get<0, 1>() +
			Get<0, 1>() * m.Get<1, 1>() +
			Get<0, 2>() * m.Get<2, 1>() +
			Get<0, 3>() * m.Get<3, 1>());
		result.Set<0, 2>(Get<0, 0>() * m.Get<0, 2>() +
			Get<0, 1>() * m.Get<1, 2>() +
			Get<0, 2>() * m.Get<2, 2>() +
			Get<0, 3>() * m.Get<3, 2>());
		result.Set<0, 3>(Get<0, 0>() * m.Get<0, 3>() +
			Get<0, 1>() * m.Get<1, 3>() +
			Get<0, 2>() * m.Get<2, 3>() +
			Get<0, 3>() * m.Get<3, 3>());
		result.Set<1, 0>(Get<1, 0>() * m.Get<0, 0>() +
			Get<1, 1>() * m.Get<1, 0>() +
			Get<1, 2>() * m.Get<2, 0>() +
			Get<1, 3>() * m.Get<3, 0>());
		result.Set<1, 1>(Get<1, 0>() * m.Get<0, 1>() +
			Get<1, 1>() * m.Get<1, 1>() +
			Get<1, 2>() * m.Get<2, 1>() +
			Get<1, 3>() * m.Get<3, 1>());
		result.Set<1, 2>(Get<1, 0>() * m.Get<0, 2>() +
			Get<1, 1>() * m.Get<1, 2>() +
			Get<1, 2>() * m.Get<2, 2>() +
			Get<1, 3>() * m.Get<3, 2>());
		result.Set<1, 3>(Get<1, 0>() * m.Get<0, 3>() +
			Get<1, 1>() * m.Get<1, 3>() +
			Get<1, 2>() * m.Get<2, 3>() +
			Get<1, 3>() * m.Get<3, 3>());
		result.Set<2, 0>(Get<2, 0>() * m.Get<0, 0>() +
			Get<2, 1>() * m.Get<1, 0>() +
			Get<2, 2>() * m.Get<2, 0>() +
			Get<2, 3>() * m.Get<3, 0>());
		result.Set<2, 1>(Get<2, 0>() * m.Get<0, 1>() +
			Get<2, 1>() * m.Get<1, 1>() +
			Get<2, 2>() * m.Get<2, 1>() +
			Get<2, 3>() * m.Get<3, 1>());
		result.Set<2, 2>(Get<2, 0>() * m.Get<0, 2>() +
			Get<2, 1>() * m.Get<1, 2>() +
			Get<2, 2>() * m.Get<2, 2>() +
			Get<2, 3>() * m.Get<3, 2>());
		result.Set<2, 3>(Get<2, 0>() * m.Get<0, 3>() +
			Get<2, 1>() * m.Get<1, 3>() +
			Get<2, 2>() * m.Get<2, 3>() +
			Get<2, 3>() * m.Get<3, 3>());
		result.Set<3, 0>(Get<3, 0>() * m.Get<0, 0>() +
			Get<3, 1>() * m.Get<1, 0>() +
			Get<3, 2>() * m.Get<2, 0>() +
			Get<3, 3>() * m.Get<3, 0>());
		result.Set<3, 1>(Get<3, 0>() * m.Get<0, 1>() +
			Get<3, 1>() * m.Get<1, 1>() +
			Get<3, 2>() * m.Get<2, 1>() +
			Get<3, 3>() * m.Get<3, 1>());
		result.Set<3, 2>(Get<3, 0>() * m.Get<0, 2>() +
			Get<3, 1>() * m.Get<1, 2>() +
			Get<3, 2>() * m.Get<2, 2>() +
			Get<3, 3>() * m.Get<3, 2>());
		result.Set<3, 3>(Get<3, 0>() * m.Get<0, 3>() +
			Get<3, 1>() * m.Get<1, 3>() +
			Get<3, 2>() * m.Get<2, 3>() +
			Get<3, 3>() * m.Get<3, 3>());

		return result;
	}

	constexpr Mat4& Mat4::operator*=(const Mat<4, 4>& m) noexcept
	{
		Mat4 result{};

		result.Set<0, 0>(Get<0, 0>() * m.Get<0, 0>() +
			Get<0, 1>() * m.Get<1, 0>() +
			Get<0, 2>() * m.Get<2, 0>() +
			Get<0, 3>() * m.Get<3, 0>());
		result.Set<0, 1>(Get<0, 0>() * m.Get<0, 1>() +
			Get<0, 1>() * m.Get<1, 1>() +
			Get<0, 2>() * m.Get<2, 1>() +
			Get<0, 3>() * m.Get<3, 1>());
		result.Set<0, 2>(Get<0, 0>() * m.Get<0, 2>() +
			Get<0, 1>() * m.Get<1, 2>() +
			Get<0, 2>() * m.Get<2, 2>() +
			Get<0, 3>() * m.Get<3, 2>());
		result.Set<0, 3>(Get<0, 0>() * m.Get<0, 3>() +
			Get<0, 1>() * m.Get<1, 3>() +
			Get<0, 2>() * m.Get<2, 3>() +
			Get<0, 3>() * m.Get<3, 3>());
		result.Set<1, 0>(Get<1, 0>() * m.Get<0, 0>() +
			Get<1, 1>() * m.Get<1, 0>() +
			Get<1, 2>() * m.Get<2, 0>() +
			Get<1, 3>() * m.Get<3, 0>());
		result.Set<1, 1>(Get<1, 0>() * m.Get<0, 1>() +
			Get<1, 1>() * m.Get<1, 1>() +
			Get<1, 2>() * m.Get<2, 1>() +
			Get<1, 3>() * m.Get<3, 1>());
		result.Set<1, 2>(Get<1, 0>() * m.Get<0, 2>() +
			Get<1, 1>() * m.Get<1, 2>() +
			Get<1, 2>() * m.Get<2, 2>() +
			Get<1, 3>() * m.Get<3, 2>());
		result.Set<1, 3>(Get<1, 0>() * m.Get<0, 3>() +
			Get<1, 1>() * m.Get<1, 3>() +
			Get<1, 2>() * m.Get<2, 3>() +
			Get<1, 3>() * m.Get<3, 3>());
		result.Set<2, 0>(Get<2, 0>() * m.Get<0, 0>() +
			Get<2, 1>() * m.Get<1, 0>() +
			Get<2, 2>() * m.Get<2, 0>() +
			Get<2, 3>() * m.Get<3, 0>());
		result.Set<2, 1>(Get<2, 0>() * m.Get<0, 1>() +
			Get<2, 1>() * m.Get<1, 1>() +
			Get<2, 2>() * m.Get<2, 1>() +
			Get<2, 3>() * m.Get<3, 1>());
		result.Set<2, 2>(Get<2, 0>() * m.Get<0, 2>() +
			Get<2, 1>() * m.Get<1, 2>() +
			Get<2, 2>() * m.Get<2, 2>() +
			Get<2, 3>() * m.Get<3, 2>());
		result.Set<2, 3>(Get<2, 0>() * m.Get<0, 3>() +
			Get<2, 1>() * m.Get<1, 3>() +
			Get<2, 2>() * m.Get<2, 3>() +
			Get<2, 3>() * m.Get<3, 3>());
		result.Set<3, 0>(Get<3, 0>() * m.Get<0, 0>() +
			Get<3, 1>() * m.Get<1, 0>() +
			Get<3, 2>() * m.Get<2, 0>() +
			Get<3, 3>() * m.Get<3, 0>());
		result.Set<3, 1>(Get<3, 0>() * m.Get<0, 1>() +
			Get<3, 1>() * m.Get<1, 1>() +
			Get<3, 2>() * m.Get<2, 1>() +
			Get<3, 3>() * m.Get<3, 1>());
		result.Set<3, 2>(Get<3, 0>() * m.Get<0, 2>() +
			Get<3, 1>() * m.Get<1, 2>() +
			Get<3, 2>() * m.Get<2, 2>() +
			Get<3, 3>() * m.Get<3, 2>());
		result.Set<3, 3>(Get<3, 0>() * m.Get<0, 3>() +
			Get<3, 1>() * m.Get<1, 3>() +
			Get<3, 2>() * m.Get<2, 3>() +
			Get<3, 3>() * m.Get<3, 3>());

		*this = result;
		return *this;
	}

	inline constexpr Vec<4> Mat4::operator*(const Vec<4> & v) const noexcept
	{
		Vec<4>	result;

		result[0] = Get<0, 0>() * v[0] + Get<0, 1>() * v[1] +
			Get<0, 2>() * v[2] + Get<0, 3>() * v[3];
		result[1] = Get<1, 0>() * v[0] + Get<1, 1>() * v[1] +
			Get<1, 2>() * v[2] + Get<1, 3>() * v[3];
		result[2] = Get<2, 0>() * v[0] + Get<2, 1>() * v[1] +
			Get<2, 2>() * v[2] + Get<2, 3>() * v[3];
		result[3] = Get<3, 0>() * v[0] + Get<3, 1>() * v[1] +
			Get<3, 2>() * v[2] + Get<3, 3>() * v[3];

		return result;
	}

	inline constexpr Vec4 Mat4::operator*(const Vec4& v) const noexcept
	{
		Vec4	result;

		result.x = Get<0, 0>() * v.x + Get<0, 1>() * v.y +
			Get<0, 2>() * v.z + Get<0, 3>() * v.w;
		result.y = Get<1, 0>() * v.x + Get<1, 1>() * v.y +
			Get<1, 2>() * v.z + Get<1, 3>() * v.w;
		result.z = Get<2, 0>() * v.x + Get<2, 1>() * v.y +
			Get<2, 2>() * v.z + Get<2, 3>() * v.w;
		result.w = Get<3, 0>() * v.x + Get<3, 1>() * v.y +
			Get<3, 2>() * v.z + Get<3, 3>() * v.w;

		return result;
	}

	template <unsigned Line, unsigned Column, typename>
	inline Mat<4, Column>	Mat4::operator* (const Mat<Line, Column>& m) const noexcept
	{
		Mat<4, Column>	result{};
		unsigned j;
		for (j = 0; j < Column; ++j)
		{
			result.Set(0, j, Get(0, 0) * m.Get(0, j) + Get(0, 1) * m.Get(1, j) +
				Get(0, 2) * m.Get(2, j) + Get(0, 3) * m.Get(3, j));
			result.Set(1, j, Get(1, 0) * m.Get(0, j) + Get(1, 1) * m.Get(1, j) +
				Get(1, 2) * m.Get(2, j) + Get(1, 3) * m.Get(3, j));
			result.Set(2, j, Get(2, 0) * m.Get(0, j) + Get(2, 1) * m.Get(1, j) +
				Get(2, 2) * m.Get(2, j) + Get(2, 3) * m.Get(3, j));
			result.Set(3, j, Get(3, 0) * m.Get(0, j) + Get(3, 1) * m.Get(1, j) +
				Get(3, 2) * m.Get(2, j) + Get(3, 3) * m.Get(3, j));
		}

		return result;
	}

	constexpr float	Mat4::Det() const noexcept
	{
		float temp0{ Get<2, 2>() * Get<3, 3>() - Get<2, 3>() * Get<3, 2>() };
		float temp1{ Get<1, 2>() * Get<3, 3>() - Get<1, 3>() * Get<3, 2>() };
		float temp2{ Get<1, 2>() * Get<2, 3>() - Get<1, 3>() * Get<2, 2>() };
		float temp3{ Get<0, 2>() * Get<3, 3>() - Get<0, 3>() * Get<3, 2>() };
		float temp4{ Get<0, 2>() * Get<2, 3>() - Get<0, 3>() * Get<2, 2>() };
		float temp5{ Get<0, 2>() * Get<1, 3>() - Get<0, 3>() * Get<1, 2>() };
		return Get<0, 0>() * (Get<1, 1>() * temp0 - Get<2, 1>() * temp1 + Get<3, 1>() * temp2)
			- Get<1, 0>() * (Get<0, 1>() * temp0 - Get<2, 1>() * temp3 + Get<3, 1>() * temp4)
			+ Get<2, 0>() * (Get<0, 1>() * temp1 - Get<1, 1>() * temp3 + Get<3, 1>() * temp5)
			- Get<3, 0>() * (Get<0, 1>() * temp2 - Get<1, 1>() * temp4 + Get<2, 1>() * temp5);
	}

	constexpr Mat4	Mat4::Inversed() const noexcept
	{
		Mat4 result;
		float temp0{ Get<2, 2>() * Get<3, 3>() - Get<2, 3>() * Get<3, 2>() };
		float temp1{ Get<1, 2>() * Get<3, 3>() - Get<1, 3>() * Get<3, 2>() };
		float temp2{ Get<1, 2>() * Get<2, 3>() - Get<1, 3>() * Get<2, 2>() };
		float temp3{ Get<0, 2>() * Get<3, 3>() - Get<0, 3>() * Get<3, 2>() };
		float temp4{ Get<0, 2>() * Get<2, 3>() - Get<0, 3>() * Get<2, 2>() };
		float temp5{ Get<0, 2>() * Get<1, 3>() - Get<0, 3>() * Get<1, 2>() };
		float det[4]{ Get<1, 1>() * temp0 - Get<2, 1>() * temp1 + Get<3, 1>() * temp2 ,
						Get<0, 1>() * temp0 - Get<2, 1>() * temp3 + Get<3, 1>() * temp4 ,
						Get<0, 1>() * temp1 - Get<1, 1>() * temp3 + Get<3, 1>() * temp5 ,
						Get<0, 1>() * temp2 - Get<1, 1>() * temp4 + Get<2, 1>() * temp5 };

		float determinant{ Get<0, 0>() * det[0] - Get<1, 0>() * det[1] + Get<2, 0>() * det[2] - Get<3, 0>() * det[3] };
		const float RDet{ 1.f / determinant };

		result.Set<0, 0>(RDet * det[0]);
		result.Set<0, 1>(-RDet * det[1]);
		result.Set<0, 2>(RDet * det[2]);
		result.Set<0, 3>(-RDet * det[3]);

		result.Set<1, 0>(-RDet * (Get<1, 0>() * temp0 - Get<2, 0>() * temp1 + Get<3, 0>() * temp2));
		result.Set<1, 1>(RDet * (Get<0, 0>() * temp0 - Get<2, 0>() * temp3 + Get<3, 0>() * temp4));
		result.Set<1, 2>(-RDet * (Get<0, 0>() * temp1 - Get<1, 0>() * temp3 + Get<3, 0>() * temp5));
		result.Set<1, 3>(RDet * (Get<0, 0>() * temp2 - Get<1, 0>() * temp4 + Get<2, 0>() * temp5));

		float temp6{ Get<2, 1>() * Get<3, 3>() - Get<2, 3>() * Get<3, 1>() };
		float temp7{ Get<1, 1>() * Get<3, 3>() - Get<1, 3>() * Get<3, 1>() };
		float temp8{ Get<1, 1>() * Get<2, 3>() - Get<1, 3>() * Get<2, 1>() };
		float temp9{ Get<0, 1>() * Get<3, 3>() - Get<0, 3>() * Get<3, 1>() };
		float temp10{ Get<0, 1>() * Get<2, 3>() - Get<0, 3>() * Get<2, 1>() };
		float temp11{ Get<0, 1>() * Get<1, 3>() - Get<0, 3>() * Get<1, 1>() };

		result.Set<2, 0>(RDet * (Get<1, 0>() * temp6 - Get<2, 0>() * temp7 + Get<3, 0>() * temp8));
		result.Set<2, 1>(-RDet * (Get<0, 0>() * temp6 - Get<2, 0>() * temp9 + Get<3, 0>() * temp10));
		result.Set<2, 2>(RDet * (Get<0, 0>() * temp7 - Get<1, 0>() * temp9 + Get<3, 0>() * temp11));
		result.Set<2, 3>(-RDet * (Get<0, 0>() * temp8 - Get<1, 0>() * temp10 + Get<2, 0>() * temp11));

		float temp12{ Get<2, 1>() * Get<3, 2>() - Get<2, 2>() * Get<3, 1>() };
		float temp13{ Get<1, 1>() * Get<3, 2>() - Get<1, 2>() * Get<3, 1>() };
		float temp14{ Get<1, 1>() * Get<2, 2>() - Get<1, 2>() * Get<2, 1>() };
		float temp15{ Get<0, 1>() * Get<3, 2>() - Get<0, 2>() * Get<3, 1>() };
		float temp16{ Get<0, 1>() * Get<2, 2>() - Get<0, 2>() * Get<2, 1>() };
		float temp17{ Get<0, 1>() * Get<1, 2>() - Get<0, 2>() * Get<1, 1>() };

		result.Set<3, 0>(-RDet * (Get<1, 0>() * temp12 - Get<2, 0>() * temp13 + Get<3, 0>() * temp14));
		result.Set<3, 1>(RDet * (Get<0, 0>() * temp12 - Get<2, 0>() * temp15 + Get<3, 0>() * temp16));
		result.Set<3, 2>(-RDet * (Get<0, 0>() * temp13 - Get<1, 0>() * temp15 + Get<3, 0>() * temp17));
		result.Set<3, 3>(RDet * (Get<0, 0>() * temp14 - Get<1, 0>() * temp16 + Get<2, 0>() * temp17));

		return result;
	}

	constexpr void	Mat4::Inverse()
	{
		*this = Inversed();
	}

	inline std::string					Mat4::ToString() const noexcept
	{
		std::string	s;
		for (unsigned i{ 0 }, j{ 0 }; i < 4; ++i)
		{
			for (j = 0; j < 4; ++j)
				s += std::to_string(Get(i, j)) + '\t';
			s.push_back('\n');
		}

		return s;
	}

	inline void							Mat4::Print() const noexcept
	{
		for (unsigned i{ 0 }, j{ 0 }; i < 4; ++i)
		{
			for (j = 0; j < 4; ++j)
				std::cout << Get(i, j) << '\t';
			std::cout << std::endl;
		}
	}
}

#endif

