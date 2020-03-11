#include "DrawableComponent.h"

namespace Editor::Datastructure
{
	unsigned int DrawableComponent::_ID_COMPONENT_INCREMENT = 0;

	DrawableComponent::DrawableComponent(std::string name) :
		m_name{ name }
	{
		m_ID = "##" + std::to_string(_ID_COMPONENT_INCREMENT++);
	}

	const std::string& DrawableComponent::GetID() const
	{
		return m_ID;
	}

	const std::string& DrawableComponent::GetName() const
	{
		return m_name;
	}

	std::string DrawableComponent::GetNameID() const
	{
		return m_name + m_ID;
	}
}
