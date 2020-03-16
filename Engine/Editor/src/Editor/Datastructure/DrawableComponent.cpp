#include "DrawableComponent.h"

namespace Editor::Datastructure
{
	unsigned int DrawableComponent::_NB_COMPONENTS = 0;

	DrawableComponent::DrawableComponent(const std::string& name) :
		m_name{ name }
	{
		m_ID = "##" + std::to_string(_NB_COMPONENTS++);
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
