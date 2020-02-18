#include "Object.hpp"

namespace Core::Datastructure
{
	const Transform& Object::GetUpdatedTransform() noexcept
	{
		if (m_transform.IsGPosUpdated())
			m_transform.UpdatePos(m_parent->GetUpdatedTransform());
		return m_transform;
	}

	Object::Object(const Transform& localPos, Object* parent) noexcept : m_transform {localPos}, m_parent {parent}
	{
		if (parent != nullptr)
			m_transform.UpdatePos(parent->GetUpdatedTransform());
	}

	Object* Datastructure::Object::CreateChild(const Transform& localPos) noexcept
	{
		m_childs.push_back(new Object(localPos, this));
		return m_childs.back();
	}

	Object* Datastructure::Object::CreateRootNode() noexcept
	{
		return new Object({}, nullptr);
	}
}
