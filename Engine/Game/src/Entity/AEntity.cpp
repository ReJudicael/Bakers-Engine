#include "AEntity.h"

RTTR_PLUGIN_REGISTRATION
{
	registration::class_<AEntity>("Entity")
		.property("Health", &AEntity::m_health)
		.property("Damage", &AEntity::m_damage);
}

AEntity::AEntity() : IUpdatable()
{
}

AEntity::~AEntity()
{
}

void AEntity::TakeDamage(const int damage)
{
	m_health -= damage;
	IsHit();
}