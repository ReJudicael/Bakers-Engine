#include "IComponent.hpp"
#include "RootObject.hpp"

namespace Core::Datastructure
{
	void	IComponent::SetScene(RootObject* scene)
	{
		m_root = scene;
		if (!m_isInit)
			m_root->AddStart(this);
	}

	SystemManagement::InputSystem* IComponent::Input() noexcept
	{
		return m_root->GetInput();
	}
}