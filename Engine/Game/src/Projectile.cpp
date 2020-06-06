#include <imgui\imgui.h>

#include "Projectile.h"
#include "Object.hpp"
#include "RootObject.hpp"
#include "Brioche.h"

RTTR_PLUGIN_REGISTRATION
{
	ZoneScopedN("Registering RTTR")
		ZoneText("Leak happening in this zone is from RTTR and is actually reflexion data. It is a purposeful leak", 98);
	registration::class_<Projectile>("Projectile")
		.constructor();
}


Projectile::Projectile() : ComponentUpdatable()
{}

Projectile::~Projectile()
{
}

void	Projectile::OnCopy(IComponent* copyTo) const
{
	ZoneScoped
		ComponentUpdatable::OnCopy(copyTo);

	Projectile* copy = dynamic_cast<Projectile*>(copyTo);
}

void	Projectile::StartCopy(IComponent*& copyTo) const
{
	ZoneScoped
		copyTo = new Projectile();
	OnCopy(copyTo);
}

bool	Projectile::OnStart()
{
	ZoneScoped
		m_parent->GetComponentsOfType<Core::Renderer::ParticleSystem>(m_particles);

	std::list<Core::Physics::SphereCollider*> colliders;
	m_parent->GetComponentsOfTypeInObject<Core::Physics::SphereCollider>(colliders);
	if (colliders.size() > 0)
		m_collider = *colliders.begin();

	if (m_collider)
		m_collider->OnTriggerEnterEvent += BIND_EVENT(Projectile::ReachTarget);

	return ComponentUpdatable::OnStart();
}

void    Projectile::OnDestroy()
{
	ComponentUpdatable::OnDestroy();
}

void    Projectile::OnReset()
{
	ComponentUpdatable::OnReset();
}

void	Projectile::OnInit()
{
	ComponentUpdatable::OnInit();
}

void	Projectile::OnUpdate(float deltaTime)
{
	if (m_isDestroyed)
		return;

	m_parent->TranslateGlobal(m_movement * m_speed * deltaTime);

	m_remainingTime -= deltaTime;

	bool toDestroy = true;
	for (auto it{ m_particles.begin() }; it != m_particles.end(); ++it)
	{
		Core::Renderer::ParticleSystem* p = *it;
		if (!p->IsOver())
			toDestroy = false;
		if (m_remainingTime <= 0)
			p->Activate(false);
	}

	if (toDestroy)
		m_parent->Destroy();
}

void	Projectile::ReachTarget(Core::Physics::Collider* collider)
{
	if (m_isDestroyed)
		return;

	std::list<AEntity*> entities;
	collider->GetParent()->GetComponentsOfBaseType<AEntity>(entities);
	if (entities.size() > 0)
	{
		// Check if the entity is the one who created the projectile
		std::list<Brioche*> caster;
		collider->GetParent()->GetComponentsOfType<Brioche>(caster);
		if (caster.size() > 0)
			return;

		AEntity* target = *entities.begin();
		target->TakeDamage(m_damage);
	}
	
	for (auto it{ m_particles.begin() }; it != m_particles.end(); ++it)
	{
		Core::Renderer::ParticleSystem* p = *it;
		p->Activate(false);
	}
}
