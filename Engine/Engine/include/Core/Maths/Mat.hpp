#ifndef __MAT__
#define __MAT__

#include <string>
#include <cmath>
#include <utility>
#include <iostream>

namespace Core::Maths
{
	/**
	 * Matrix class with templated size
	 * @tparam Line: Number of lines in the matrix
	 * @tparam Colomn: Number of colomns in the matrix
	 */
	template<unsigned Line, unsigned Column>
	struct Mat
	{
		/**
		 * Number of elements in the matrix
		 */
		static constexpr unsigned Size{ Line * Column };

		union
		{
			float				m_mat[Size];
			float				mat[Line][Column];
		};

		/**
		 * Generates matrix zero
		 */
		inline constexpr							Mat() noexcept;

		/**
		 * Generates matrix with values generated by a pattern multiplied by d
		 * @param d: Multiplyer of the pattern
		 */
		inline constexpr							Mat(int d) noexcept;

		/**
		 * Creates a copy of the matrix
		 * @param m: Matrix to be copied
		 */
		inline constexpr							Mat(const Mat<Line, Column>& m) noexcept;

		/**
		 * Moves a matrix to another memory location
		 * @param m: Matrix to be moved
		 */
		inline constexpr							Mat(Mat<Line, Column>&& m) noexcept;

		/**
		 * Returns the number of lines of this matrix
		 * @return Number of lines of the matrix
		 */
		inline constexpr unsigned					Lines() const noexcept;

		/**
		 * Returns the number of colomns of this matrix
		 * @return Number of colomns of the matrix
		 */
		inline constexpr unsigned					Columns() const noexcept;


		/**
		 * Returns an identity matrix of size SecLine and SecColomn.
		 * The number of lines and columns must be the same
		 * @tparam SecLine: Number of lines of the matrix
		 * @tparam SecColumn: Number of columns of the matrix
		 * @return Identity matrix
		 */
		template <unsigned SecLine, unsigned SecColumn = SecLine, typename = std::enable_if_t<SecLine == SecColumn>>
		inline constexpr Mat<SecLine, SecColumn>	Identity() const noexcept;

		/**
		 * Returns an identity matrix of the same size 
		 * as the class it was called from
		 * @return Identity matrix
		 */
		template <typename = std::enable_if_t<Line == Column>>	
		inline constexpr Mat<Line, Column>			Identity() const noexcept;

		/**
		 * Transpose current matrix
		 */
		template <typename = std::enable_if_t<Line == Column>>
		inline constexpr void						Transpose() noexcept;
		/**
		 * Returns transposed version of current matrix
		 * @return current matrix transposed
		 */
		[[nodiscard]] inline constexpr Mat<Column, Line>	Transposed() const noexcept;

		/**
		 * Returns a sub-matrix of current matrix, where
		 * the specified line and column are removed
		 * @param line: line to remove
		 * @param col: column to remove
		 * @return sub-matrix of current matrix
		 */
		inline constexpr Mat<Line - 1, Column - 1>	GetSubMatrix(const unsigned line, const unsigned col) const noexcept;
		/**
		 * Returns a sub-matrix of current matrix, where
		 * the specified line and column are removed
		 * @tparam l: line to remove
		 * @tparam c: column to remove
		 * @return sub-matrix of current matrix
		 */
		template <unsigned l, unsigned c>
		inline constexpr Mat<Line - 1, Column - 1>	GetSubMatrix() const noexcept;
	protected:
		/**
		 * Implementation of GetSubMatrix(), private member you're
		 * not supposed to use.
		 * @param result: matrix to emplace the result of the operation
		 */
		template <unsigned l, unsigned c, unsigned cL = 0, unsigned cC = 0, unsigned wL = 0, unsigned wC = 0>
		__forceinline constexpr void				GetSubMatrix_IMPL(Mat<Line - 1, Column - 1> & result) const noexcept;
	public:

		/**
		 * Computes the determinant of the current matrix
		 * @return The determinant of the current matrix
		 */
		template <int l = Line, int c = Column, typename = std::enable_if_t<l == c && l >= 5U>>
		inline constexpr std::enable_if_t<l >= 5U, float>	Det() const noexcept;

		/**
		 * Computes the determinant of the current matrix
		 * @return The determinant of the current matrix
		 */
		template <int l = Line, int c = Column, typename = std::enable_if_t<(l == c) && (l == 1)>>
		inline constexpr std::enable_if_t<l == 1U, float>	Det() const noexcept;

		/**
		 * Computes the determinant of the current matrix
		 * @return The determinant of the current matrix
		 */
		template <int l = Line, int c = Column, typename = std::enable_if_t<l == c && l == 2>>
		inline constexpr std::enable_if_t<l == 2U, float>	Det() const noexcept;
		/**
		 * Computes the determinant of the current matrix
		 * @return The determinant of the current matrix
		 */
		template <int l = Line, int c = Column, typename = std::enable_if_t<l == c && l == 3>>
		inline constexpr std::enable_if_t<l == 3U, float>	Det() const noexcept;
		/**
		 * Computes the determinant of the current matrix
		 * @return The determinant of the current matrix
		 */
		template <int l = Line, int c = Column, typename = std::enable_if_t<l == c && l == 4>>
		inline constexpr std::enable_if_t<l == 4U, float>	Det() const noexcept;

		/**
		 * Computes the cofactor of the current matrix
		 * @param l: the line of the cofactor
		 * @param c: the column of the cofactor
		 * @return The cofactor of the current matrix
		 */
		inline constexpr float				GetCofactor(const unsigned l, const unsigned c) const noexcept;
		/**
		 * Computes the cofactor of the current matrix
		 * @tparam l: the line of the cofactor
		 * @tparam c: the column of the cofactor
		 * @return The cofactor of the current matrix
		 */
		template <unsigned l, unsigned c>
		inline constexpr float				GetCofactor() const noexcept;
		/**
		 * Computes the minor of the current matrix
		 * @param l: the line of the minor
		 * @param c: the column of the minor
		 * @return The minor of the current matrix
		 */
		inline constexpr float				GetMinor(const unsigned l, const unsigned c) const noexcept;
		/**
		 * Computes the minor of the current matrix
		 * @tparam l: the line of the minor
		 * @tparam c: the column of the minor
		 * @return The minor of the current matrix
		 */
		template <unsigned l, unsigned c>
		inline constexpr float				GetMinor() const noexcept;
		/**
		 * Computes the cofactor matrix of the current matrix
		 * @return The cofactor matrix of the current matrix
		 */
		inline constexpr Mat<Line, Column>	GetCofactorMatrix() const noexcept;
	protected:
		/**
		 * Implementation of GetCofactorMatrix(), private member you're
		 * not supposed to use.
		 * @param result: matrix to emplace the result of the operation
		 */
		template <unsigned l = 0, unsigned c = 0>
		__forceinline constexpr void		GetCofactorMatrix_IMPL(Mat<Line, Column> & result) const noexcept;
	public:
		/**
		 * Inverses current matrix
		 */
		template <typename = std::enable_if_t<Line == Column>>
		inline constexpr void				Inverse();
		
		/**
		 * Computes inverse of current matrix
		 * @result The inverse of the current matrix
		 */
		template <typename = std::enable_if_t<Line == Column>>
		inline constexpr Mat<Line, Column>	Inversed() const noexcept;

		/**
		 * Print current matrix to the console
		 */
		inline void							Print() const noexcept;
		/**
		 * Returns the matrix in a string format, with a tabulation 
		 * between each number and end line at the end of each line
		 * @return The string representation of the current matrix
		 */
		inline std::string					ToString() const noexcept;

		/**
		 * Getter that takes as argument only the 
		 * position in the array of the desired value
		 * @param a: Position in the array
		 * @return Value requested
		 */
		inline constexpr float				Get(const unsigned a) const noexcept;
		/**
		 * Getter that takes as argument only the
		 * position in the array of the desired value
		 * @param a: Position in the array
		 * @return Reference to value requested
		 */
		inline constexpr float& Get(const unsigned a) noexcept;
		/**
		 * Getter that takes the requested position
		 * in the matrix as template
		 * @return Value requested
		 */
		template <unsigned l, unsigned c, typename = std::enable_if_t<l <= Line - 1 && c <= Column - 1>>
		inline constexpr float				Get() const noexcept;
		/**
		 * Getter that takes the requested position
		 * in the matrix as parameter
		 * @param line: Line of the requested value
		 * @param column: Column of the requested value
		 * @return Value requested
		 */
		inline constexpr float				Get(const unsigned line, const unsigned column) const noexcept;
		/**
		 * Getter that takes the requested position
		 * in the matrix as parameter
		 * @param line: Line of the requested value
		 * @param column: Column of the requested value
		 * @return Reference to value requested
		 */
		inline constexpr float& Get(const unsigned line, const unsigned column) noexcept;
		/**
		 * Setter that takes the requested position
		 * in the matrix as template
		 * @tparam l: Line of the value to be set
		 * @tparam c: Column of the value to be set
		 * @param f: Value to be set
		 */
		template <unsigned l, unsigned c, typename = std::enable_if_t<l <= Line - 1 && c <= Column - 1>>
		inline constexpr void				Set(const float f) noexcept;
		/**
		 * Setter that takes the requested position
		 * in the matrix as parameter
		 * @param line: Line of the value to be set
		 * @param column: Column of the value to be set
		 * @param f: Value to be set
		 */
		inline constexpr void				Set(const unsigned line, const unsigned column, const float f) noexcept;
		/**
		 * Setter that takes the requested position
		 * in the array as parameter
		 * @param a: Position of the value to be set
		 * @param f: Value to be set
		 */
		inline constexpr void				Set(const unsigned a, const float f) noexcept;
		/**
		 * Getter that takes the requested position 
		 * in the array as parameter.
		 * @param a: Position of the requested value
		 * @result Value requested
		 */
		inline constexpr float				operator() (const unsigned a) const noexcept;
		/**
		 * Getter that takes the requested position 
		 * in the array as parameter.
		 * @param a: Position of the requested value
		 * @result Reference to value requested
		 */
		inline constexpr float&				operator() (const unsigned a) noexcept;
		/**
		 * Getter that takes the requested position 
		 * in the matrix as parameter.
		 * @param line: Line of the requested value
		 * @param column: Column of the requested value
		 * @result Value requested
		 */
		inline constexpr float				operator() (const unsigned line, const unsigned column) const noexcept;
		/**
		 *Getter that takes the requested position
		 * in the matrix as parameter.
		 * @param line: Line of the requested value
		 * @param column: Column of the requested value
		 * @result Reference to value requested
		 */
		inline constexpr float& operator() (const unsigned line, const unsigned column) noexcept;

		/**
		 * Adds the values of the two matrices
		 * @param m: Matrix to add
		 * @return Matrix that represents the addition of the two matrices
		 */
		inline constexpr Mat<Line, Column>	operator+ (const Mat<Line, Column>& m) const noexcept;
		/**
		 * Adds the value of the given matrix to
		 * the value of the current matrix
		 * @param m: The matrix to add
		 * @return Result of the addition
		 */
		inline constexpr Mat<Line, Column>	operator+= (const Mat<Line, Column>& m) noexcept;
		/**
		 * Substracts the values of the two matrices
		 * @param m: Matrix to substract
		 * @return Matrix that represents the Substraction of the two matrices
		 */
		inline constexpr Mat<Line, Column>	operator- (const Mat<Line, Column>& m) const noexcept;
		/**
		 * Substracts the value of the given matrix
		 * to the value of the current matrix
		 * @param m: Matrix to substract
		 * @return Matrix that represents the Substraction of the two matrices
		 */
		inline constexpr Mat<Line, Column>	operator-= (const Mat<Line, Column>& m) noexcept;
		/**
		 * Multiplies the values of the two matrices
		 * @param m: Matrix to multiply
		 * @return Matrix that represents the multiplication of the two matrices
		 */
		template <unsigned SecLine, unsigned SecColumn, typename = std::enable_if_t<SecLine == Column>>
		inline constexpr Mat<Line, SecColumn>	operator* (const Mat<SecLine, SecColumn>& m) const noexcept;
		/**
		 * Multiplies the value of the given matrix
		 * to the value of the current matrix
		 * @param m: Matrix to multiply
		 * @return Matrix that represents the multiplication of the twi matrices
		 */
		template <unsigned SecLine, unsigned SecColumn, typename = std::enable_if_t<SecLine == Column>>
		inline constexpr Mat<Line, SecColumn>	operator*= (const Mat<SecLine, SecColumn>& m) noexcept;
		/**
		 * Scalar multiplication of the current matrix by a value
		 * @param f: Scalar to multiply
		 * @return Current matrix multiplied by scalar f
		 */
		inline constexpr Mat<Line, Column>	operator* (const float f) const noexcept;
		/**
		 * Scalar division of the current matrix by a value
		 * @param f: Scalar to divide by
		 * @return Current matrix divided by scalar f
		 */
		inline constexpr Mat<Line, Column>	operator/ (const float f) const noexcept;
		/**
		 * Scalar multiplication of the current matrix by a value
		 * @param f: Scalar to multiply
		 * @return Reference to current matrix
		 */
		inline constexpr Mat<Line, Column>	operator*= (const float f) noexcept;
		/**
		 * Checks if two matrices are equal
		 * @param m: Matrix to compare to
		 * @result Boolean are matrix equals
		 */
		inline constexpr bool				operator== (const Mat<Line, Column>& m) const noexcept;
		/**
		 * Checks if two matrices are not equal
		 * @param m: Matrix to compare to
		 * @result Boolean are matrix not equals
		 */
		inline constexpr bool				operator!= (const Mat<Line, Column>& m) const noexcept;
		/**
		 * Copies the given matrix in current matrix
		 * @param m: Matrix to copy
		 * @return A copy of the matrix
		 */
		inline constexpr Mat<Line, Column>	operator= (const Mat<Line, Column>& m) noexcept;
		/** 
		 * Moves the given matrix to the current matrix
		 * @param m: Matrix to move
		 * @return A copy of the matrix
		 */
		inline constexpr Mat<Line, Column>	operator= (Mat<Line, Column>&& m) noexcept;
	protected:
		/**
		 * Implementation of operator* (const Mat<SecLine, SecColumn>&), 
		 * private member you're not supposed to use.
		 * @param result: matrix to emplace the result of the operation
		 */
		template<unsigned SecLine, unsigned SecColumn, unsigned line = 0, unsigned inter = 0, unsigned column = 0>

		__forceinline constexpr void		operatorMultiply_IMPL(const Mat<Line, Column> & other, Mat<Line, Column> & result) const noexcept;
	public:
	};

	template <unsigned Line, unsigned Column>
	constexpr	Mat<Line, Column>::Mat() noexcept : m_mat()
	{
		for (unsigned i{ 0 }; i < Size; ++i)
			m_mat[i] = 0;
	}

	template <unsigned Line, unsigned Column>
	constexpr	Mat<Line, Column>::Mat(int d) noexcept : m_mat()
	{
		for (unsigned i{ 0 }; i < Size; ++i)
		{
			if (i % 3 != 0)
				m_mat[i] = 0;
			else
				m_mat[i] = (float)i * d;
		}
	}

	template <unsigned Line, unsigned Column>
	constexpr	Mat<Line, Column>::Mat(const Mat<Line, Column>& m) noexcept : m_mat()
	{
		for (unsigned i{ 0 }; i < Size; ++i)
			m_mat[i] = m(i);
	}

	template <unsigned Line, unsigned Column>
	constexpr	Mat<Line, Column>::Mat(Mat<Line, Column>&& m) noexcept : m_mat()
	{
		for (unsigned i{ 0 }; i < Line * Column; ++i)
			m_mat[i] = std::move(m(i));
	}

	template <unsigned Line, unsigned Column>
	constexpr unsigned	Mat<Line, Column>::Lines() const noexcept
	{
		return Line;
	}

	template <unsigned Line, unsigned Column>
	constexpr unsigned	Mat<Line, Column>::Columns() const noexcept
	{
		return Column;
	}

	template<unsigned Line, unsigned Column>
	template<unsigned SecLine, unsigned SecColumn, typename>
	constexpr Mat<SecLine, SecColumn> Maths::Mat<Line, Column>::Identity() const noexcept
	{
		Mat<SecLine, SecColumn>	m;
		for (unsigned i{ 0 }; i < Line; ++i)
			m.Set(i, i, 1.f);
		return m;
	}

	template<unsigned Line, unsigned Column>
	template<typename>
	constexpr Mat<Line, Column> Maths::Mat<Line, Column>::Identity() const noexcept
	{
		Mat<Line, Column>	m;
		for (unsigned i{ 0 }; i < Line; ++i)
			m.Set(i, i, 1.f);
		return m;
	}

	template<unsigned Line, unsigned Column>
	constexpr Mat<Column, Line> Maths::Mat<Line, Column>::Transposed() const noexcept
	{
		Mat<Column, Line>	m;

		for (unsigned i{ 0 }, j{ 0 }; i < Line; ++i)
			for (j = 0; j < Column; ++j)
				m.Set(j, i, Get(i, j));

		return m;
	}

	template<unsigned Line, unsigned Column>
	template<typename>
	constexpr void Maths::Mat<Line, Column>::Transpose() noexcept
	{
		float	f;
		for (unsigned i{ 1 }, j{ 1 }; i < Line; ++i)
			for (j = i - 1; j < Column; ++j)
			{
				f = Get(i, j);
				Set(i, j, Get(j, i));
				Set(j, i, f);
			}
	}

	template<unsigned Line, unsigned Column>
	constexpr Mat<Line - 1, Column - 1>	Mat<Line, Column>::GetSubMatrix(const unsigned line, const unsigned col) const noexcept
	{
		Mat<Line - 1, Column - 1>	m{};
		for (unsigned i{ 0 }, j{ 0 }, k{ 0 }, l{ 0 }; i < Line; ++i)
		{
			if (i == line)
				continue;
			for (l = j = 0; j < Column; ++j)
			{
				if (j == col)
					continue;
				m.Set(k, l, Get(i, j));
				++l;
			}
			++k;
		}

		return m;
	}

	template<unsigned Line, unsigned Column>
	template <unsigned l, unsigned c>
	constexpr Mat<Line - 1, Column - 1>	Mat<Line, Column>::GetSubMatrix() const noexcept
	{
		Mat<Line - 1, Column - 1>	result;
		GetSubMatrix_IMPL<l, c>(result);
		return result;
	}

#ifdef _DEBUG
	template<unsigned Line, unsigned Column>
	template <unsigned l, unsigned c, unsigned cL, unsigned cC, unsigned wL, unsigned wC>
	constexpr void	Mat<Line, Column>::GetSubMatrix_IMPL(Mat<Line - 1, Column - 1> & result) const noexcept
	{
		for (unsigned i{ 0 }, j{ 0 }, k{ 0 }, z{ 0 }; i < Line; ++i)
		{
			if (i == l)
				continue;
			for (z = j = 0; j < Column; ++j)
			{
				if (j == c)
					continue;
				result.Set(k, z, Get(i, j));
				++z;
			}
			++k;
		}
	}
#else
	template<unsigned Line, unsigned Column>
	template <unsigned l, unsigned c, unsigned cL, unsigned cC, unsigned wL, unsigned wC>
	__forceinline constexpr void	Mat<Line, Column>::GetSubMatrix_IMPL(Mat<Line - 1, Column - 1> & result) const noexcept
	{
		if constexpr (cL == Line)
			return;
		else if constexpr (cC == Column)
		{
			GetSubMatrix_IMPL<l, c, cL + 1, 0, wL + 1, 0>(result);
			return;
		}
		else if constexpr (cL == l)
		{
			GetSubMatrix_IMPL<l, c, cL + 1, 0, wL, 0>(result);
			return;
		}
		else if constexpr (cC == c)
		{
			GetSubMatrix_IMPL<l, c, cL, cC + 1, wL, wC>(result);
		}
		else
		{
			result.Set<wL, wC>(Get<cL, cC>());
			GetSubMatrix_IMPL<l, c, cL, cC + 1, wL, wC + 1>(result);
		}
	}
#endif
	template<unsigned Line, unsigned Column>
	template<int l, int c, typename>
	constexpr std::enable_if_t<l >= 5U, float> Maths::Mat<Line, Column>::Det() const noexcept
	{
		float det{ 0 };
		for (unsigned i{ 0 }; i < c; ++i)
		{
			if (i % 2 == 0)
				det += Get(0, i) * GetSubMatrix(0, i).Det();
			else
				det -= Get(0, i) * GetSubMatrix(0, i).Det();
		}
		return det;
	}


	template<unsigned Line, unsigned Column>
	template<int l, int c, typename>
	constexpr std::enable_if_t<l == 1U, float> Maths::Mat<Line, Column>::Det() const noexcept
	{
		return Get<0, 0>();
	}

	template<unsigned Line, unsigned Column>
	template<int l, int c, typename>
	constexpr std::enable_if_t<l == 2U, float> Maths::Mat<Line, Column>::Det() const noexcept
	{
		return Get<0, 0>() * Get<1, 1>() - Get<1, 0>() * Get<0, 1>();
	}

	template<unsigned Line, unsigned Column>
	template<int l, int c, typename>
	constexpr std::enable_if_t<l == 3U, float> Maths::Mat<Line, Column>::Det() const noexcept
	{
		return Get<0, 0>() * Get<1, 1>() * Get<2, 2>()
			+ Get<0, 1>() * Get<1, 2>() * Get<2, 0>()
			+ Get<0, 2>() * Get<1, 0>() * Get<2, 1>()
			- Get<0, 2>() * Get<1, 1>() * Get<2, 0>()
			- Get<0, 1>() * Get<1, 0>() * Get<2, 2>()
			- Get<0, 0>() * Get<1, 2>() * Get<2, 1>();
	}

	template<unsigned Line, unsigned Column>
	template<int l, int c, typename>
	constexpr std::enable_if_t<l == 4U, float> Maths::Mat<Line, Column>::Det() const noexcept
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

	template<unsigned Line, unsigned Column>
	template<unsigned l, unsigned c>
	inline constexpr float Mat<Line, Column>::GetCofactor() const noexcept
	{
		if constexpr ((l + c) % 2 == 0)
			return GetSubMatrix<l, c>().Det();
		else
			return -GetSubMatrix<l, c>().Det();
	}

	template<unsigned Line, unsigned Column>
	template<unsigned l, unsigned c>
	inline constexpr float Mat<Line, Column>::GetMinor() const noexcept
	{
		return GetSubMatrix<l, c>().Det();
	}

	template<unsigned Line, unsigned Column>
	constexpr float	Maths::Mat<Line, Column>::GetCofactor(const unsigned l, const unsigned c) const noexcept
	{
		if ((l + c) % 2 == 0)
			return GetSubMatrix(l, c).Det();
		else
			return -GetSubMatrix(l, c).Det();
	}

	template<unsigned Line, unsigned Column>
	inline constexpr float Maths::Mat<Line, Column>::GetMinor(const unsigned l, const unsigned c) const noexcept
	{
		return GetSubMatrix(l, c).Det();
	}

	template<unsigned Line, unsigned Column>
	inline constexpr Mat<Line, Column> Maths::Mat<Line, Column>::GetCofactorMatrix() const noexcept
	{
		Mat<Line, Column>	m{};

		GetCofactorMatrix_IMPL(m);

		return m;
	}

#ifdef _DEBUG
	template<unsigned Line, unsigned Column>
	template<unsigned l, unsigned c>
	inline constexpr void Mat<Line, Column>::GetCofactorMatrix_IMPL(Mat<Line, Column>& result) const noexcept
	{
		for (unsigned i{ 0 }, j{ 0 }; i < Line; ++i)
			for (j = 0; j < Column; ++j)
				result.Set(i, j, GetCofactor(i, j));
	}
#else
	template<unsigned Line, unsigned Column>
	template<unsigned l, unsigned c>
	inline constexpr void Mat<Line, Column>::GetCofactorMatrix_IMPL(Mat<Line, Column>& result) const noexcept
	{
		if constexpr (l == Line)
		{
			return;
		}
		else if constexpr (c == Column)
		{
			GetCofactorMatrix_IMPL<l + 1, 0>(result);
			return;
		}
		else
		{
			result.Set<l, c>(GetCofactor<l, c>());
			GetCofactorMatrix_IMPL<l, c + 1>(result);
		}
	}
#endif
	template<unsigned Line, unsigned Column>
	template<typename>
	inline constexpr void Maths::Mat<Line, Column>::Inverse()
	{
		float	det{ Det() };
		if (det == 0)
			throw "Mat cannot be inversed: det is 0";

		*this = 
			((GetCofactorMatrix().Transposed()) / det);
	}

	template<unsigned Line, unsigned Column>
	template<typename>
	inline constexpr Mat<Line, Column> Maths::Mat<Line, Column>::Inversed() const noexcept
	{
		float	det{ Det() };
		if (det == 0)
			return Mat<Line, Column>();
		return (GetCofactorMatrix().Transposed() / det);
	}

	template<unsigned Line, unsigned Column>
	inline void Maths::Mat<Line, Column>::Print() const noexcept
	{
		for (unsigned i{ 0 }, j{ 0 }; i < Line; ++i)
		{
			for (j = 0; j < Column; ++j)
				std::cout << Get(i, j) << '\t';
			std::cout << std::endl;
		}
	}

	template<unsigned Line, unsigned Column>
	inline std::string Maths::Mat<Line, Column>::ToString() const noexcept
	{
		std::string	s;
		for (unsigned i{ 0 }, j{ 0 }; i < Line; ++i)
		{
			for (j = 0; j < Column; ++j)
				s += std::to_string(Get(i, j)) + '\t';
			s.push_back('\n');
		}

		return s;
	}

	template<unsigned Line, unsigned Column>
	inline constexpr float Maths::Mat<Line, Column>::Get(const unsigned a) const noexcept
	{
		return m_mat[a];
	}

	template<unsigned Line, unsigned Column>
	inline constexpr float& Maths::Mat<Line, Column>::Get(const unsigned a) noexcept
	{
		return m_mat[a];
	}


	template<unsigned Line, unsigned Column>
	template <unsigned l, unsigned c, typename>
	inline constexpr float Maths::Mat<Line, Column>::Get() const noexcept
	{
		constexpr unsigned pos{ l * Column + c };
		return m_mat[pos];
	}

	template<unsigned Line, unsigned Column>
	inline constexpr float Maths::Mat<Line, Column>::Get(const unsigned line, const unsigned column) const noexcept
	{
		return m_mat[line * Column + column];
	}

	template<unsigned Line, unsigned Column>
	inline constexpr float& Maths::Mat<Line, Column>::Get(const unsigned line, const unsigned column) noexcept
	{
		return m_mat[line * Column + column];
	}

	template<unsigned Line, unsigned Column>
	template <unsigned l, unsigned c, typename>
	inline constexpr void Maths::Mat<Line, Column>::Set(const float f) noexcept
	{
		constexpr unsigned pos{ l * Column + c };
		m_mat[pos] = f;
	}

	template<unsigned Line, unsigned Column>
	inline constexpr void Maths::Mat<Line, Column>::Set(const unsigned line, const unsigned column, const float f) noexcept
	{
		m_mat[line * Column + column] = f;
	}

	template<unsigned Line, unsigned Column>
	inline constexpr void Maths::Mat<Line, Column>::Set(const unsigned a, const float f) noexcept
	{
		m_mat[a] = f;
	}

	template<unsigned Line, unsigned Column>
	inline constexpr float Maths::Mat<Line, Column>::operator()(const unsigned a) const noexcept
	{
		return m_mat[a];
	}

	template<unsigned Line, unsigned Column>
	inline constexpr float& Maths::Mat<Line, Column>::operator()(const unsigned a) noexcept
	{
		return m_mat[a];
	}

	template<unsigned Line, unsigned Column>
	inline constexpr float Maths::Mat<Line, Column>::operator()(const unsigned line, const unsigned column) const noexcept
	{
		return m_mat[line * Column + column];
	}

	template<unsigned Line, unsigned Column>
	inline constexpr float& Maths::Mat<Line, Column>::operator()(const unsigned line, const unsigned column) noexcept
	{
		return m_mat[line * Column + column];
	}

	template<unsigned Line, unsigned Column>
	inline constexpr Mat<Line, Column> Core::Maths::Mat<Line, Column>::operator+(const Mat<Line, Column>& m) const noexcept
	{
		Mat<Line, Column>	result{ m };

		for (unsigned i{ 0 }; i < Size; ++i)
				result(i) += Get(i);
		return result;
	}
	
	template<unsigned Line, unsigned Column>
	inline constexpr Mat<Line, Column> Core::Maths::Mat<Line, Column>::operator+=(const Mat<Line, Column>& m) noexcept
	{
		for (unsigned i{ 0 }; i < Size; ++i)
			Get(i) += m(i);
		return *this;
	}
	
	template<unsigned Line, unsigned Column>
	inline constexpr Mat<Line, Column> Core::Maths::Mat<Line, Column>::operator-(const Mat<Line, Column>& m) const noexcept
	{
		Mat<Line, Column>	result{ *this };

		for (unsigned i{ 0 }; i < Size; ++i)
			result(i) -= Get(i);
		return result;
	}
	
	template<unsigned Line, unsigned Column>
	inline constexpr Mat<Line, Column> Core::Maths::Mat<Line, Column>::operator-=(const Mat<Line, Column>& m) noexcept
	{
		for (unsigned i{ 0 }; i < Size; ++i)
			Get(i) -= m(i);
		return *this;
	}

	template<unsigned Line, unsigned Column>
	template<unsigned SecLine, unsigned SecColumn, typename>
	inline constexpr Mat<Line, SecColumn> Maths::Mat<Line, Column>::operator*(const Mat<SecLine, SecColumn>& m) const noexcept
	{
		Mat<Line, SecColumn>	result{};

		operatorMultiply_IMPL<SecLine, SecColumn>(m, result);

		return result;
	}

	template<unsigned Line, unsigned Column>
	template<unsigned SecLine, unsigned SecColumn, typename>
	inline constexpr Mat<Line, SecColumn> Core::Maths::Mat<Line, Column>::operator*=(const Mat<SecLine, SecColumn>& m) noexcept
	{
		Mat<Line, SecColumn>	result{};
		
		operatorMultiply_IMPL<SecLine, SecColumn>(m, result);

		*this = result;
		return *this;
	}

#ifdef _DEBUG
	template<unsigned Line, unsigned Column>
	template<unsigned SecLine, unsigned SecColumn, unsigned line, unsigned inter, unsigned column>
	__forceinline constexpr void Mat<Line, Column>::operatorMultiply_IMPL(const Mat<Line, Column>& other, Mat<Line, Column>& result) const noexcept
	{
		for (unsigned i{ 0 }, j{ 0 }, k{ 0 }; i < Line; ++i)
			for (j = 0; j < SecColumn; ++j)
				for (k = 0; k < Column; ++k)
					result(i, j) += Get(i, k) * other(k, j);
	}
#else
	template<unsigned Line, unsigned Column>
	template<unsigned SecLine, unsigned SecColumn, unsigned line, unsigned inter, unsigned column>
	__forceinline constexpr void Mat<Line, Column>::operatorMultiply_IMPL(const Mat<Line, Column>& other, Mat<Line, Column>& result) const noexcept
	{
		if constexpr (line == Line)
			return;
		else if constexpr (inter == SecColumn)
		{
			operatorMultiply_IMPL<SecLine, SecColumn, line + 1, 0, 0>(other, result);
			return;
		}
		else if constexpr (column == Column)
		{
			operatorMultiply_IMPL<SecLine, SecColumn, line, inter + 1, 0>(other, result);
			return;
		}
		else
		{
			result.Set<line, column>(result.Get<line, column>() + Get<line, inter>() * other.Get<inter, column>());
			operatorMultiply_IMPL<SecLine, SecColumn, line, inter, column + 1>(other, result);
		}
	}
#endif
	template<unsigned Line, unsigned Column>
	inline constexpr Mat<Line, Column> Maths::Mat<Line, Column>::operator*(const float f) const noexcept
	{
		Mat<Line, Column>	m{ *this };
		for (unsigned i{ 0 }, j{ 0 }; i < Line; ++i)
			for (j = 0; j < Column; ++j)
				m(i, j) *= f;

		return m;
	}
	
	template<unsigned Line, unsigned Column>
	inline constexpr Mat<Line, Column> Core::Maths::Mat<Line, Column>::operator/(const float f) const noexcept
	{
		Mat<Line, Column>	m{ *this };
		for (unsigned i{ 0 }; i < Line * Column; ++i)
			m.m_mat[i] /= f;

		return m;
	}
	
	template<unsigned Line, unsigned Column>
	inline constexpr Mat<Line, Column> Core::Maths::Mat<Line, Column>::operator*=(const float f) noexcept
	{
		for (unsigned i{ 0 }, j{ 0 }; i < Line; ++i)
			for (j = 0; j < Column; ++j)
				Get(i, j) *= f;
		return *this;
	}
	
	template<unsigned Line, unsigned Column>
	inline constexpr bool Core::Maths::Mat<Line, Column>::operator==(const Mat<Line, Column>& m) const noexcept
	{
		for (unsigned i{ 0 }, j{ 0 }; i < Line; ++i)
			for (j = 0; j < Column; ++j)
				if (Get(i, j) != m(i, j))
					return false;
		return true;
	}
	
	template<unsigned Line, unsigned Column>
	inline constexpr bool Core::Maths::Mat<Line, Column>::operator!=(const Mat<Line, Column>& m) const noexcept
	{
		for (unsigned i{ 0 }, j{ 0 }; i < Line; ++i)
			for (j = 0; j < Column; ++j)
				if (Get(i, j) != m(i, j))
					return true;
		return false;
	}
	
	template<unsigned Line, unsigned Column>
	inline constexpr Mat<Line, Column> Core::Maths::Mat<Line, Column>::operator=(const Mat<Line, Column>& m) noexcept
	{
		for (unsigned i{ 0 }, j{ 0 }; i < Line; ++i)
			for (j = 0; j < Column; ++j)
				Get(i, j) = m(i, j);

		return *this;
	}
	
	template<unsigned Line, unsigned Column>
	inline constexpr Mat<Line, Column> Mat<Line, Column>::operator=(Mat<Line, Column>&& m) noexcept
	{
		for (unsigned i{ 0 }; i < Line * Column; ++i)
			m_mat[i] = std::move(m.m_mat[i]);
		return *this;
	}
}




#endif