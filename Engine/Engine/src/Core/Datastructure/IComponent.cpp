#include "IComponent.hpp"
#include "InputSystem.hpp"
#include "RootObject.hpp"
#include "CoreMinimal.h"

RTTR_REGISTRATION
{
	using namespace Core::Datastructure;
	registration::class_<IComponent>("IComponent")
		.property("parent", &IComponent::GetParent, &IComponent::SetParent, detail::protected_access())
		.property("Scene", &IComponent::GetScene, &IComponent::SetScene, detail::protected_access())
		.property_readonly("InputManager", &IComponent::Input, detail::protected_access())
		.method("Start", &IComponent::Start)
		.method("Destroy", &IComponent::Destroy)
		.property_readonly("IsDestroyed", &IComponent::IsDestroyed)
		.property_readonly("IsInit", &IComponent::IsInit)
		.property("IsActive", &IComponent::m_isActive, detail::protected_access());
}

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
		if (m_isInit || !m_isActive || IsDestroyed())
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