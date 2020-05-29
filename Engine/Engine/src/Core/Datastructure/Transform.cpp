#include "Transform.hpp"
#include "Debug.h "

RTTR_PLUGIN_REGISTRATION
{
	ZoneScopedN("Registering RTTR")
	using namespace Core::Datastructure;
	using namespace Core::Maths;
	registration::class_<Transform>("Transform")
		.constructor<const Transform&, const Vec3&, const Quat&, const Vec3&>()
		.constructor<const Vec3&, const Quat&, const Vec3&>()
		.constructor<const Transform&>()
		.property("LocalPos", &Transform::GetLocalPos, &Transform::SetLocalPos)
		.property("LocalRot", &Transform::GetLocalRot, &Transform::SetLocalRot)
		.property("LocalScale", &Transform::GetLocalScale, &Transform::SetLocalScale)
		.property_readonly("GlobalPos", &Transform::GetGlobalPos)
		.property_readonly("GlobalRot", &Transform::GetGlobalRot)
		.property_readonly("GlobalScale", &Transform::GetGlobalScale)
		.method("GetLocalTRS", &Transform::GetLocalTrs)
		.method("GetGlobalTRS", &Transform::GetGlobalTrs)
		.property_readonly("IsGposUpdated", &Transform::m_isGUpdated)
		.property_readonly("IsGTrsUpdated", &Transform::m_isGTrsUpdated)
		.property_readonly("IsLTrsUpdated", &Transform::m_isTrsUpdated);

	lua.new_usertype<Transform>("Transform",
		sol::constructors<Transform()>(),
		"Position", &Transform::m_pos,
		"Rotation", & Transform::m_rot,
		"Scale", &Transform::m_scale,
		"GlobalPos", &Transform::m_gpos,
		"GlobalRot", &Transform::m_grot,
		"GlobalScale", &Transform::m_gscale,
		"Translate", &Transform::Translate,
		"Rotate", &Transform::Rotate,
		"Scale", &Transform::Scale,
		"Right", &Transform::GetRight,
		"Up", &Transform::GetUp,
		"Forward", &Transform::GetForward,
		"RotateTowards", &Transform::RotateTowards);
}

namespace Core::Datastructure
{
	inline void Transform::UpdatePos(const Transform& t) noexcept
	{
		if (m_isGUpdated)
			return;
		ZoneScoped
			ZoneText("Updating transform", 19)
		m_gpos = t.GetGlobalPos() + (t.GetGlobalRot() * Maths::Quat(0, m_pos * t.GetGlobalScale()) * t.GetGlobalRot().InversedUnit()).xyz;
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

	const Maths::Vec3& Transform::Translate(const Maths::Vec3& v) noexcept
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
	void Transform::RotateGlobal(const Transform& parent, const Maths::Quat& q) noexcept
	{
		m_isTrsUpdated = false;
		m_isGTrsUpdated = false;
		m_isGUpdated = false;

		m_rot *= parent.GetGlobalRot().Inversed() * q;
		m_rot.Normalize();
	}
	void Transform::ScaleGlobal(const Transform& parent, const Maths::Vec3& v) noexcept
	{
		m_isTrsUpdated = false;
		m_isGTrsUpdated = false;
		m_isGUpdated = false;

		m_scale *= v / parent.GetGlobalScale();
	}
	void Transform::TranslateGlobal(const Transform& parent, const Maths::Vec3& v) noexcept
	{
		m_isTrsUpdated = false;
		m_isGTrsUpdated = false;
		m_isGUpdated = false;

		m_pos += (parent.GetGlobalRot().Inversed() * Core::Maths::Quat(0, v - parent.GetGlobalPos()) * parent.GetGlobalRot()).GetVec() / parent.GetGlobalScale();
	}
	const Maths::Mat4& Transform::GetLocalTrs() noexcept
	{
		if (m_isTrsUpdated)
			return m_TRS;

		ZoneScoped
			ZoneText("Updating local TRS", 19)
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

		ZoneScoped
			ZoneText("Updating global TRS", 20)
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

	void Transform::RotateTowards(const Maths::Vec3& target, const float& rotation)
	{
		Maths::Vec3 forward = GetForward();

		if ((forward - target).SquaredLength() < 0.001)
			return;

		Maths::Quat targetRotation = m_rot * Maths::Quat::FromToRotation(forward, target);
		SetLocalRot(m_rot.Slerp(targetRotation, rotation));
	}

	Transform::Transform(const Maths::Vec3& pos, const Maths::Quat& rot, const Maths::Vec3& scale) noexcept : m_pos {pos}, m_rot {rot}, m_scale {scale}
	{
	}

	Transform::Transform(const Transform& parent, const Maths::Vec3& pos, const Maths::Quat& rot, const Maths::Vec3& scale) noexcept : m_pos {pos}, m_rot {rot}, m_scale {scale}
	{
		UpdatePos(parent);
	}
	Transform::Transform(const Transform& t) noexcept : 
		m_pos {t.m_pos}, m_rot {t.m_rot}, m_scale {t.m_scale}, m_TRS {t.m_TRS},
		m_gpos {t.m_gpos}, m_grot {t.m_grot}, m_gscale {t.m_gscale}, m_gTRS {t.m_gTRS}, 
		m_isGTrsUpdated {t.m_isGTrsUpdated}, m_isGUpdated {t.m_isGUpdated}, m_isTrsUpdated {t.m_isTrsUpdated}
	{
	}
}