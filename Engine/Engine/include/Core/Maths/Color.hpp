#pragma once

#ifndef __COLOR__
#define __COLOR__

#include "Vec4.hpp"

namespace Core::Maths
{
	/**
	 * Color class with predefined size of 4 (r, g, b, a)
	 */
	struct Color
	{
		union
		{
			float		rgba[4];
			struct
			{
				float	r;
				float	g;
				float	b;
				float	a;
			};
			struct
			{
				float	rg[2];
				float	ba[2];
			};
			struct
			{
				float	r;
				float	gba[3];
			};
			struct
			{
				float	rgb[3];
				float	a;
			};
		};

		/**
		 * Default constructor, creates a white color
		 */
		inline constexpr	Color() noexcept : r{ 1 }, g{ 1 }, b{ 1 }, a{ 1 }
		{
		}

		/**
		 * Creates a color with the given values
		 * @param r: The red component of the vector
		 * @param g: The green component of the vector
		 * @param b: The blue component of the vector
		 * @param a: The alpha component of the vector
		 */
		inline constexpr	Color(float r, float g, float b, float a = 1) noexcept : r{ r }, g{ g }, b{ b }, a{ a }
		{
		}

		/**
		 * Creates a copy of given color
		 * @param c: Color to be copied
		 */
		inline constexpr	Color(const Color& c) noexcept : r{ c.r }, g{ c.g }, b{ c.b }, a{ c.a }
		{
		}

		/**
		 * Cast operator to Vec4
		 */
		inline constexpr	operator Core::Maths::Vec4() const noexcept
		{
			return Core::Maths::Vec4(r, g, b, a);
		}

		/**
		 * Copies the given color in current color
		 * @param c: Color to copy
		 * @return A copy of the given color
		 */
		inline constexpr Color	operator=(const Color& v) noexcept
		{
			r = v.r;
			g = v.g;
			b = v.b;
			a = v.a;

			return *this;
		}

		/**
		 * Compares the given color to the current color
		 * @param c: Color to compare to
		 * @return The result of the comparison
		 */
		inline constexpr	bool operator==(const Color& c) const noexcept
		{
			return r == c.r && g == c.g && b == c.b && a == c.a;
		}

		/**
		 * Compares the given color to the current color
		 * @param c: Color to compare to
		 * @return The result of the comparison
		 */
		inline constexpr	bool operator!=(const Color& c) const noexcept
		{
			return !operator==(c);
		}

		inline	std::string ToString() const noexcept
		{
			return std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + " " + std::to_string(a);
		}
	};
}

#endif
