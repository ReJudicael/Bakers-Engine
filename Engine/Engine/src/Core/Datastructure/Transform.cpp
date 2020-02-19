#include "Transform.hpp"

namespace Core::Datastructure
{
	inline void Transform::UpdatePos(const Transform& t) noexcept
	{
		if (m_isGUpdated)
			return;
		m_gpos = t.GetGlobalPos() + (t.GetGlobalRot() * Maths::Quat(0, m_pos) * t.GetGlobalRot().InversedUnit()).xyz;
		m_grot = (t.GetGlobalRot() * m_rot).Normalized();
		m_gscale = m_scale * t.GetGlobalScale();
		m_isGUpdated = true;
	}

	void Transform::UpdatePos() noexcept
	{
		if (m_isGUpdated)
			return;
		m_gpos = m_pos;
		m_grot = m_rot;
		m_gscale = m_scale;
		m_isGUpdated = true;
	}

	inline const Maths::Vec3& Transform::Translate(const Maths::Vec3& v) noexcept
	{
		m_isTrsUpdated = false;
		m_isGTrsUpdated = false;
		m_isGUpdated = false;

		m_pos += v;
		return m_pos;
	}
	const Maths::Quat& Transform::Rotate(const Maths::Quat& q) noexcept
	{
		m_isTrsUpdated = false;
		m_isGTrsUpdated = false;
		m_isGUpdated = false;

		m_rot *= q;
		m_rot.Normalize();
		return m_rot;
	}
	const Maths::Vec3& Transform::Scale(const Maths::Vec3& v) noexcept
	{
		m_isTrsUpdated = false;
		m_isGTrsUpdated = false;
		m_isGUpdated = false;

		m_scale *= v;
		return m_scale;
	}
	const Maths::Mat4& Transform::GetLocalTrs() noexcept
	{
		if (m_isTrsUpdated)
			return m_TRS;

		constexpr Maths::Mat<4, 4> identity{ identity.Identity() };
		Maths::Mat4 rot{ m_rot.GetMatUnit() };
		
		Maths::Mat4 trans{ identity };
		trans.Set<0, 3>(m_pos.x);
		trans.Set<1, 3>(m_pos.y);
		trans.Set<2, 3>(m_pos.z);

		Maths::Mat4 scale{};
		scale.Set<0, 0>(m_scale.x);
		scale.Set<1, 1>(m_scale.y);
		scale.Set<2, 2>(m_scale.z);
		scale.Set<3, 3>(1.f);

		m_TRS = trans * rot * scale;
		m_isTrsUpdated = true;
		return m_TRS;
	}
	const Maths::Mat4& Transform::GetGlobalTrs() noexcept
	{
		if (m_isGTrsUpdated)
			return m_gTRS;

		constexpr Maths::Mat<4, 4> identity{ identity.Identity() };
		Maths::Mat4 rot{ m_grot.GetMatUnit() };

		Maths::Mat4 trans{ identity };
		trans.Set<0, 3>(m_gpos.x);
		trans.Set<1, 3>(m_gpos.y);
		trans.Set<2, 3>(m_gpos.z);

		Maths::Mat4 scale{};
		scale.Set<0, 0>(m_gscale.x);
		scale.Set<1, 1>(m_gscale.y);
		scale.Set<2, 2>(m_gscale.z);
		scale.Set<3, 3>(1.f);

		m_gTRS = trans * rot * scale;
		m_isGTrsUpdated = true;
		return m_gTRS;
	}

	Transform::Transform(const Maths::Vec3& pos, const Maths::Quat& rot, const Maths::Vec3& scale) noexcept : m_pos {pos}, m_rot {rot}, m_scale {scale}
	{
	}

	Transform::Transform(const Transform& parent, const Maths::Vec3& pos, const Maths::Quat& rot, const Maths::Vec3& scale) noexcept : m_pos {pos}, m_rot {rot}, m_scale {scale}
	{
		UpdatePos(parent);
	}
}