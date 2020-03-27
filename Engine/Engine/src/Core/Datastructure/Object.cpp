#include "Object.hpp"
#include "RootObject.hpp"
#include "CoreMinimal.h"

RTTR_REGISTRATION
{
	using namespace Core::Datastructure;
	registration::class_<Object>("Object")
		.method("Destroy", &Object::Destroy)
		.property_readonly("IsDestroyed", &Object::IsDestroyed)
		.method("CreateChild", &Object::CreateChild)
		.property_readonly("parent", &Object::GetParent)
		.method("Translate", &Object::Translate)
		.method("Rotate", &Object::Rotate)
		.method("Scale", &Object::Scale)
		.property("pos", &Object::GetPos, &Object::SetPos)
		.property("rot", &Object::GetRot, &Object::SetRot)
		.property("pos", &Object::GetGlobalPos, &Object::SetGlobalPos)
		.property("rot", &Object::GetGlobalRot, &Object::SetGlobalRot)
		.property("scale", &Object::GetGlobalScale, &Object::SetGlobalScale)
		.property_readonly("GlobalPos", &Object::GetGlobalPos)
		.property_readonly("GlobalRot", &Object::GetGlobalRot)
		.property_readonly("GlobalScale", &Object::GetGlobalScale)
		.method("GetLocalTRS", &Object::GetLocalTRS)
		.method("GetGlobalTRS", &Object::GetGlobalTRS)
		.method("AddComponent", &Object::AddComponent)
		.method("RemoveComponent", &Object::RemoveComponent)
		.method("EraseComponent", &Object::EraseComponent)
		.property_readonly("Scene", &Object::GetScene)
		.property_readonly("Forward", &Object::Forward)
		.property_readonly("Up", &Object::Up)
		.property_readonly("Right", &Object::Right);
}

namespace Core::Datastructure
{
	const Transform& Object::GetUpdatedTransform() noexcept
	{
		ZoneScoped
			ZoneText("Updating and returning updated transform", 41)
		if (!m_transform.IsGPosUpdated())
		{
			if (m_parent == this || m_parent == nullptr)
				m_transform.UpdatePos();
			else
				m_transform.UpdatePos(m_parent->GetUpdatedTransform());
		}
		return m_transform;
	}

	Object::Object(const std::string& name, const Transform& localPos, Object* parent, RootObject* scene) noexcept : m_name{ name }, m_root { scene }, m_transform{ localPos }, m_parent{ parent }
	{
		if (parent != nullptr)
			m_transform.UpdatePos(parent->GetUpdatedTransform());
		else
			m_transform.UpdatePos();
	}

	void Object::Destroy() noexcept
	{
		if (m_isDestroyed)
			return;
		m_isDestroyed = true;

		for (auto it{ m_components.begin() }; it != m_components.end(); ++it)
			(*it)->Destroy();
		for (auto it{ m_childs.begin() }; it != m_childs.end(); ++it)
			(*it)->Destroy();
		m_root->DestroyObject(this);
	}

	Object* Datastructure::Object::CreateChild(const std::string& name, const Transform& localPos) noexcept
	{
		m_childs.push_back(new Object(name, localPos, this, m_root));
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

	void	Object::RemoveChild(Object* object) noexcept
	{
		m_childs.remove(object);
	}

	bool			Object::IsChild(Object* o) const noexcept
	{
		for (auto c : m_childs)
		{
			if (c == o || c->IsChild(o))
				return true;
		}
		return false;
	}
}
