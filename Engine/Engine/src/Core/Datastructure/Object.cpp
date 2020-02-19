#include "Object.hpp"

namespace Core::Datastructure
{
	const Transform& Object::GetUpdatedTransform() noexcept
	{
		if (!m_transform.IsGPosUpdated())
		{
			if (m_parent == nullptr)
				m_transform.UpdatePos();
			else
				m_transform.UpdatePos(m_parent->GetUpdatedTransform());
		}
		return m_transform;
	}

	Object::Object(const Transform& localPos, Object* parent) noexcept : m_transform {localPos}, m_parent {parent}
	{
		if (parent != nullptr)
			m_transform.UpdatePos(parent->GetUpdatedTransform());
		else
			m_transform.UpdatePos();
	}

	Object* Datastructure::Object::CreateChild(const Transform& localPos) noexcept
	{
		m_childs.push_back(new Object(localPos, this));
		return m_childs.back();
	}

	Object* Datastructure::Object::CreateRootNode() noexcept
	{
		Transform t;
		t.UpdatePos();
		return new Object(t, nullptr);
	}

	void Object::AddComponent(ComponentBase* c) noexcept
	{
		c->SetParent(this);
		m_components.push_back(c);
	}
	void Object::RemoveComponent(ComponentBase* c) noexcept
	{
		m_components.remove(c);
	}

	void Object::EraseComponent(ComponentBase* c) noexcept
	{
		for (auto it{ m_components.begin() }; it != m_components.end(); ++it)
			if (*it == c)
			{
				(*it)->Destroy();
				return;
			}
	}
}
