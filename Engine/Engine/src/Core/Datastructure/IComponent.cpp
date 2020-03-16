#include "IComponent.hpp"
#include "InputSystem.hpp"
#include "RootObject.hpp"
#include "CoreMinimal.h"

namespace Core::Datastructure
{
	void	IComponent::SetScene(RootObject* scene)
	{
		m_root = scene;
		if (!m_isInit)
			m_root->AddStart(this);
	}

	void	IComponent::Start()
	{
		if (m_isInit || !m_isActive)
			return;
		ZoneScoped
			ZoneText("Start of a component", 21)
		OnStart();
		m_isInit = true;
	}

	void IComponent::Destroy()
	{
		if (m_isDestroyed)
			return;
		ZoneScoped
			ZoneText("Destroy of a component", 23)
		OnDestroy();
		m_root->DestroyComponent(this);
		m_isDestroyed = true;
	}

	SystemManagement::InputSystem* IComponent::Input() noexcept
	{
		return m_root->GetInput();
	}
}