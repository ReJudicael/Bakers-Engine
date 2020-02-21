#include "Object.hpp"
#include "RootObject.hpp"

namespace Core::Datastructure
{
	const Transform& Object::GetUpdatedTransform() noexcept
	{
		if (!m_transform.IsGPosUpdated())
		{
			if (m_parent == this || m_parent == nullptr)
				m_transform.UpdatePos();
			else
				m_transform.UpdatePos(m_parent->GetUpdatedTransform());
		}
		return m_transform;
	}

	Object::Object(const Transform& localPos, Object* parent, RootObject* scene) noexcept : m_root{ scene }, m_transform { localPos }, m_parent{ parent }
	{
		if (parent != nullptr)
			m_transform.UpdatePos(parent->GetUpdatedTransform());
		else
			m_transform.UpdatePos();
	}

	Object* Datastructure::Object::CreateChild(const Transform& localPos) noexcept
	{
		m_childs.push_back(new Object(localPos, this, m_root));
		return m_childs.back();
	}

	void Object::AddComponent(ComponentBase* c) noexcept
	{
		c->SetParent(this);
		c->SetScene(m_root);
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
