#include "Switch.h"

#include "EngineCore.h"
#include "RootObject.hpp"
#include "BoxCollider.h"

RTTR_PLUGIN_REGISTRATION
{
	registration::class_<Switch>("Switch")
		.constructor();
}


	Switch::Switch() : ComponentBase()
	{}

	Switch::~Switch()
	{
	}

	void	Switch::OnCopy(IComponent* copyTo) const
	{
		ZoneScoped
			ComponentBase::OnCopy(copyTo);

		Switch* copy{ dynamic_cast<Switch*>(copyTo) };

		copy->m_collider = m_collider;
		copy->m_switchActivated = m_switchActivated;
	}

	void	Switch::StartCopy(IComponent*& copyTo) const
	{
		ZoneScoped
			copyTo = new Switch();
		OnCopy(copyTo);
	}

	bool	Switch::OnStart()
	{
		ZoneScoped

		std::list<Core::Physics::BoxCollider*> colliders;
		m_parent->GetComponentsOfTypeInObject<Core::Physics::BoxCollider>(colliders);
		if (colliders.size() > 0)
			m_collider = *colliders.begin();

		if (m_collider)
		{
			m_collider->OnTriggerEnterEvent += BIND_EVENT(Switch::Activate);
			m_collider->OnTriggerExitEvent += BIND_EVENT(Switch::Deactivate);
		}

		return ComponentBase::OnStart();
	}

	void    Switch::OnDestroy()
	{
		ComponentBase::OnDestroy();
	}

	void    Switch::OnReset()
	{
		ComponentBase::OnReset();

		m_collider = nullptr;
		m_switchActivated = false;
	}

	void	Switch::OnInit()
	{
		ComponentBase::OnInit();
	}

	void	Switch::Activate(Core::Physics::Collider* collider)
	{
		m_switchActivated = true;
	}

	void	Switch::Deactivate(Core::Physics::Collider* collider)
	{
		m_switchActivated = false;
	}
