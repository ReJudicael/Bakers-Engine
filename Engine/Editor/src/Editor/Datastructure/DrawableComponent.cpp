#include "DrawableUIComponent.h"

namespace Editor::Datastructure
{
	unsigned int DrawableUIComponent::_NB_COMPONENTS = 0;

	DrawableUIComponent::DrawableUIComponent(const std::string& name) :
		m_name{ name }
	{
		m_ID = "##" + std::to_string(_NB_COMPONENTS++);
	}

	const std::string& DrawableUIComponent::GetID() const
	{
		return m_ID;
	}

	const std::string& DrawableUIComponent::GetName() const
	{
		return m_name;
	}

	std::string DrawableUIComponent::GetNameID() const
	{
		return m_name + m_ID;
	}
}
