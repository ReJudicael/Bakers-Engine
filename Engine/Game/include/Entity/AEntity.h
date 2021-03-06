#pragma once

#include "CoreMinimal.h"
#include "IUpdatable.hpp"

BAKERS_GAME_CLASS AEntity : public Core::Datastructure::IUpdatable
{
public:
	int m_health{ 0 };
	int m_damage{ 0 };

public:
	/**
	 * Default constructor
	 */
	AEntity();

	/**
	 * Destructor
	 */
	~AEntity();

	virtual void TakeDamage(const int damage);

	virtual void IsHit() {};

	REGISTER_CLASS(Core::Datastructure::IUpdatable)
};

