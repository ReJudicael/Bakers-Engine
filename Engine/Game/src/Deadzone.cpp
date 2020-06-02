#include "Deadzone.h"
#include "Object.hpp"
#include "AEntity.h"
#include <Brioche.h>

RTTR_PLUGIN_REGISTRATION
{
	registration::class_<Deadzone>("Deadzone")
		.constructor();
}


Deadzone::Deadzone() : ComponentBase()
{}

Deadzone::~Deadzone()
{
}

void	Deadzone::OnCopy(IComponent* copyTo) const
{
	ZoneScoped
		ComponentBase::OnCopy(copyTo);

	Deadzone* copy{ dynamic_cast<Deadzone*>(copyTo) };

	copy->m_collider = m_collider;
}

void	Deadzone::StartCopy(IComponent*& copyTo) const
{
	ZoneScoped
		copyTo = new Deadzone();
	OnCopy(copyTo);
}

bool	Deadzone::OnStart()
{
	ZoneScoped

	std::list<Core::Physics::BoxCollider*> colliders;
	m_parent->GetComponentsOfTypeInObject<Core::Physics::BoxCollider>(colliders);
	if (colliders.size() > 0)
		m_collider = *colliders.begin();

	if (m_collider)
	{
		m_collider->OnTriggerEnterEvent += BIND_EVENT(Deadzone::Activate);
		m_collider->OnTriggerExitEvent += BIND_EVENT(Deadzone::Deactivate);
	}

	return ComponentBase::OnStart();
}

void    Deadzone::OnDestroy()
{
	ComponentBase::OnDestroy();
}

void    Deadzone::OnReset()
{
	ComponentBase::OnReset();
}

void	Deadzone::OnInit()
{
	ComponentBase::OnInit();
}

void	Deadzone::Activate(Core::Physics::Collider* collider)
{
	std::list<AEntity*> entities;
	collider->GetParent()->GetComponentsOfBaseTypeInObject<AEntity>(entities);
	for (auto& entity : entities)
		entity->m_health = 0.f;	
}

void	Deadzone::Deactivate(Core::Physics::Collider* collider)
{
}
