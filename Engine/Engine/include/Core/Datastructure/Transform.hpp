#pragma once

#include "Vec3.hpp"
#include "Quaternion.hpp"
#include "Mat4.hpp"
#include "CoreMinimal.h"

namespace Core::Datastructure
{
	/**
	 * Computes local and global TRS, global rotation, scale and position
	 */
	BAKERS_API_CLASS Transform
	{
	protected:
		//Local pos
		Maths::Vec3 m_pos;
		Maths::Quat m_rot;
		Maths::Vec3 m_scale;

		//Local TRS
		Maths::Mat4	m_TRS;

		//Global pos
		Maths::Vec3 m_gpos;
		Maths::Quat m_grot;
		Maths::Vec3 m_gscale;

		//Global TRS
		Maths::Mat4	m_gTRS;

		//Boolean controlling updates of values
		bool		m_isTrsUpdated{ false };
		bool		m_isGUpdated{ false };
		bool		m_isGTrsUpdated{ false };
	public:
		/**
		 * Transform constructor that takes as argument the local component of
		 * a transform
		 * @param pos: Local position of the transform
		 * @param rot: Local rotation of the transform
		 * @param scale: Local scale of the transform
		 */
		Transform(const Maths::Vec3& pos = { 0, 0, 0 }, const Maths::Quat& rot = {}, const Maths::Vec3& scale = {1, 1, 1}) noexcept;
		/**
		 * Transform constructor that takes as argument the parent of the transform
		 * and the local component of a transform. It initializes the global values
		 * @param parent: Parent of this transform
		 * @param pos: Local position of the transform
		 * @param rot: Local rotation of the transform
		 * @param scale: Local scale of the transform
		 */
		Transform(const Transform& parent, const Maths::Vec3& pos, const Maths::Quat& rot = {}, const Maths::Vec3& scale = {1, 1, 1}) noexcept;

		/**
		 * Copy constructor of the transform
		 * @param t: Transform to copy
		 */
		Transform(const Transform& t) noexcept;

		/**
		 * Returns local position of the transform
		 * @return Local position
		 */
		inline const Maths::Vec3&	GetLocalPos() const noexcept;
		/**
		 * Returns local rotation of the transform
		 * @return Local rotation
		 */
		inline const Maths::Quat&	GetLocalRot() const noexcept;
		/**
		 * Returns local scale of the transform
		 * @return Local scale
		 */
		inline const Maths::Vec3&	GetLocalScale() const noexcept;

		/**
		 * Updates global transform variables in the given transform referential
		 * @param t: Parent transform
		 */
		void						UpdatePos(const Transform& t) noexcept;

		/**
		 * Updates global transform variables as a RootNode
		 */
		void						UpdatePos() noexcept;

		/**
		 * Set the global transform variables to be updated
		 */
		inline void					RequireUpdate() noexcept;

		/**
		 * Returns global position of the transform
		 * @return Global position
		 */
		inline const Maths::Vec3&	GetGlobalPos() const noexcept;
		/**
		 * Returns global rotation of the transform
		 * @return Global rotation
		 */
		inline const Maths::Quat&	GetGlobalRot() const noexcept;
		/**
		 * Returns global scale of the transform
		 * @return Global scale
		 */
		inline const Maths::Vec3&	GetGlobalScale() const noexcept;

		/**
		 * Return forward of transform
		 * @return Forward of transform
		 */
		inline const Maths::Vec3 GetForward() const noexcept;

		/**
		 * Return up of transform
		 * @return Up of transform
		 */
		inline const Maths::Vec3 GetUp() const noexcept;

		/**
		 * Return right of transform
		 * @return Right of transform
		 */
		inline const Maths::Vec3 GetRight() const noexcept;

		/**
		 * Returns if the global transform variables are currently up to date
		 * @return The state of the global variables
		 */
		inline bool					IsGPosUpdated() const noexcept;
		/**
		 * Returns if the global trs is currently up to date
		 * @return The state of the global trs
		 */
		inline bool					IsGTrsUpdated() const noexcept;
		/**
		 * Returns if the local trs is currently up to date
		 * @return The state of the local trs
		 */
		inline bool					IsTrsUpdated() const noexcept;

		/**
		 * Translates transform in local referential by given vector. 
		 * Global variables and local trs needs updating after operation
		 * @param v: Vector to translate by
		 * @return Local position after operation
		 */
		const Maths::Vec3&			Translate(const Maths::Vec3& v) noexcept;
		/**
		 * Rotates transform in local referential by given vector.
		 * Global variables and local trs needs updating after operation
		 * @param q: Quaternion to rotate by
		 * @return Local rotation after operation
		 */
		const Maths::Quat&			Rotate(const Maths::Quat& q) noexcept;
		/**
		 * Scales local transform by given vector.
		 * Global variables and local trs needs updating after operation
		 * @param v: Vector to scale by
		 * @return Local rotation after operation
		 */
		const Maths::Vec3&			Scale(const Maths::Vec3& v) noexcept;

		/**
		 * Translates transform in global referential by given vector.
		 * Global variables and local trs needs updating after operation
		 * @param v: Vector to translate by
		 * @return Local position after operation
		 */
		void TranslateGlobal(const Transform& parent, const Maths::Vec3& v) noexcept;
		/**
		 * Rotates transform in global referential by given vector.
		 * Global variables and local trs needs updating after operation
		 * @param q: Quaternion to rotate by
		 * @return Local rotation after operation
		 */
		void RotateGlobal(const Transform& parent, const Maths::Quat& q) noexcept;
		/**
		 * Scales global transform by given vector.
		 * Global variables and local trs needs updating after operation
		 * @param v: Vector to scale by
		 * @return Local rotation after operation
		 */
		void ScaleGlobal(const Transform& parent, const Maths::Vec3& v) noexcept;
		
		/**
		 * Set local position to given position
		 * @param pos: New local position
		 */
		void						SetLocalPos(const Maths::Vec3& pos) noexcept
		{
			m_isTrsUpdated = false;
			m_isGUpdated = false;
			m_isGTrsUpdated = false;

			m_pos = pos;
		}
		/**
		 * Set local rotation to given rotation
		 * @param rot: New local rotation
		 */
		void						SetLocalRot(const Maths::Quat& rot) noexcept
		{
			m_isTrsUpdated = false;
			m_isGUpdated = false;
			m_isGTrsUpdated = false;

			m_rot = rot;
		}

		/**
		 * Set local scale to given scale
		 * @param scale: New local scale
		 */
		void						SetLocalScale(const Maths::Vec3& scale) noexcept
		{
			m_isTrsUpdated = false;
			m_isGUpdated = false;
			m_isGTrsUpdated = false;

			m_scale = scale;
		}

		/**
		 * Set pos to given pos
		 * @param parent: Parent of the transform
		 * @param pos: New pos
		 */
		void						SetGlobalPos(const Transform& parent, const Maths::Vec3& pos) noexcept
		{
			m_isTrsUpdated = false;
			m_isGUpdated = false;
			m_isGTrsUpdated = false;

			m_pos = (parent.GetGlobalRot().Inversed() * Core::Maths::Quat(0, pos - parent.GetGlobalPos()) * parent.GetGlobalRot()).GetVec() / parent.GetGlobalScale();
		}
		
		/**
		 * Set rot to given rot
		 * @param parent: Parent of the transform
		 * @param rot: New rot
		 */
		void						SetGlobalRot(const Transform& parent, const Maths::Quat& rot) noexcept
		{
			m_isTrsUpdated = false;
			m_isGUpdated = false;
			m_isGTrsUpdated = false;

			m_rot = parent.GetGlobalRot().Inversed() * rot;
		}

		/**
		 * Set scale to given scale
		 * @param parent: Parent of the transform
		 * @param scale: New scale
		 */
		void						SetGlobalScale(const Transform& parent, const Maths::Vec3& scale) noexcept
		{
			m_isTrsUpdated = false;
			m_isGUpdated = false;
			m_isGTrsUpdated = false;

			m_scale = scale / parent.GetGlobalScale();
		}

		/**
		 * Returns reference to local trs. Can be updated, make a copy if current trs
		 * is the required value. Will update current trs if required
		 * @return Reference to local trs
		 */
		const Maths::Mat4&			GetLocalTrs() noexcept;
		/**
		 * Returns reference to global trs. Can be updated, make a copy if current trs
		 * is the required value. Will update current trs if required
		 * @return Reference to global trs
		 */
		const Maths::Mat4& GetGlobalTrs() noexcept;

		/**
		 * Set rotation so the forward vector becomes target vector
		 * @param target: Future forward of the transform
		 * @param rotation: Slerp value to determine the rotation between current and target (must be between 0 and 1)
		 */
		void RotateTowards(const Maths::Vec3& target, const float& rotation);

		REGISTER_CLASS()
	};

	inline const Maths::Vec3& Transform::GetLocalPos() const noexcept
	{
		return m_pos;
	}
	inline const Maths::Quat& Transform::GetLocalRot() const noexcept
	{
		return m_rot;
	}
	inline const Maths::Vec3& Transform::GetLocalScale() const noexcept
	{
		return m_scale;
	}

	inline const Maths::Vec3& Transform::GetGlobalPos() const noexcept
	{
		return m_gpos;
	}
	inline const Maths::Quat& Transform::GetGlobalRot() const noexcept
	{
		return m_grot;
	}
	inline const Maths::Vec3& Transform::GetGlobalScale() const noexcept
	{
		return m_gscale;
	}

	inline bool Transform::IsGPosUpdated() const noexcept
	{
		return m_isGUpdated;
	}

	inline bool Transform::IsGTrsUpdated() const noexcept
	{
		return m_isGTrsUpdated;
	}

	inline bool Transform::IsTrsUpdated() const noexcept
	{
		return m_isTrsUpdated;
	}

	inline void Transform::RequireUpdate() noexcept
	{
		m_isGUpdated = false;
		m_isGTrsUpdated = false;
	}

	inline const Maths::Vec3 Transform::GetForward() const noexcept
	{
		return GetLocalRot() * Maths::Vec3(0.0f, 0.0f, 1.0f);
	}

	inline const Maths::Vec3 Transform::GetUp() const noexcept
	{
		return GetLocalRot() * Maths::Vec3(0.0f, 1.0f, 0.0f);
	}

	inline const Maths::Vec3 Transform::GetRight() const noexcept
	{
		return GetLocalRot() * Maths::Vec3(1.0f, 0.0f, 0.0f);
	}
}