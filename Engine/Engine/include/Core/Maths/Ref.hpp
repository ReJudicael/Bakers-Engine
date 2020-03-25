#ifndef __REF__
#define __REF__

#include "MathMinimal.h"

#include <Vec.hpp>

namespace Core::Maths
{
	/**
	 * Templated referential struct.
	 * @tparam Size: Number of dimentions of the referential
	 */
	template <int Size>
	struct Ref
	{
		Vec<Size>		origin;
		Vec<Size>		ref[Size];

		/**
		 * Default constructor of the referential, with origin at 0 and
		 * each axis the default one
		 */
		inline constexpr Ref() noexcept : origin{ 0.f }, ref{}
		{
			for (unsigned i{ 0 }; i < Size; ++i)
			{
				ref[i].m_vec[i] = 1.f;
			}
		}

		/**
		 * Copy constructor
		 * @param r: Ref to be copied
		 */
		inline constexpr Ref(const Ref<Size>& r) noexcept : origin{ r.origin }, ref{}
		{
			for (unsigned i{ 0 }; i < Size; ++i)
				ref[i] = r.Get(i);
		}

		/**
		 * Move constructor
		 * @param r: Ref to be moved
		 */
		inline constexpr Ref(Ref<Size>&& r) noexcept : origin{ std::move(r.origin) }, ref{}
		{
			for (unsigned i{ 0 }; i < Size; ++i)
				ref[i] = std::move(r.ref[i]);
		}

		/**
		 * Getter for referential axis
		 * @param i: Number of the axis requested
		 * @return Axis of the referential requested
		 */
		inline constexpr Vec<Size>&			operator[] (const unsigned& i) noexcept
		{
			return ref[i];
		}


		/**
		 * Getter for referential axis
		 * @param i: Number of the axis requested
		 * @return Axis of the referential requested
		 */
		inline constexpr Vec<Size>			operator[] (const unsigned& i) const noexcept
		{
			return ref[i];
		}


		/**
		 * Getter for referential axis
		 * @param i: Number of the axis requested
		 * @return Axis of the referential requested
		 */
		inline constexpr const Vec<Size>&	Get(const unsigned& i) const noexcept
		{
			return ref[i];
		}

		/**
		 * Setter for referential axis
		 * @param i: Number of the axis requested
		 * @param v: Value to be set
		 */
		inline constexpr void				Set(const unsigned& i, const Vec<Size>& v) noexcept
		{
			ref[i] = v;
		}

		/**
		 * Setter for referential axis
		 * @param i: Number of the axis requested
		 * @param v: Value to be moved
		 */
		inline constexpr void				Set(const unsigned& i, Vec<Size>&& v) noexcept
		{
			ref[i] = std::move(v);
		}
	};
};


#endif
