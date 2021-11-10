#pragma once

#include "CoreMinimal.h"
#include "ComponentUpdatable.h"
#include "Vec3.hpp"
#include "SphereCollider.h"
#include "ParticleSystem.h"
#include "AEntity.h"

BAKERS_GAME_CLASS Projectile : public Core::Datastructure::ComponentUpdatable
{
	bool	m_isDestroyed{ false };
	int		m_damage{ 500 };
	float	m_speed{ 1.f };
	float	m_remainingTime{ 10.f };
	Core::Physics::SphereCollider* m_collider;
	Core::Maths::Vec3	m_movement{ 0, 0, 0 };
	std::list<Core::Renderer::ParticleSystem*> m_particles;
	

	protected:
		/**
		 * Copies the data of the component into the given component.
		 * Should always be safe to cast pointer to current component type.
		 */
		virtual void	OnCopy(IComponent* copyTo) const override;
		/**
		 * Copies the component in the given pointer.
		 * On override, should not call other versions of the function.
		 */
		virtual void	StartCopy(IComponent*& copyTo) const override;

		/**
		 * First frame upon creation event
		 */
		virtual bool	OnStart() override;

		/**
		 * Destroy event
		 */
		virtual void	OnDestroy() override;

		/**
		 * Reset event
		 */
		virtual void	OnReset() override;

public:
	/**
	 * Default Constructor
	 */
	Projectile();

	/**
	 * Destructor
	 */
	~Projectile();

	/**
	 * Init event
	 */
	void	OnInit() override;

	/**
	 * Update event
	 * @param deltaTime: Time elapsed between two frames
	 */
	 virtual void OnUpdate(float deltaTime) override;

	 /**
	  * Movement setter
	  * @param value: New value for movement vector;
	  */
	 void	SetMovement(const Core::Maths::Vec3& value) { m_movement = value; }

	 /**
	  * Speed setter
	  * @param value: New value for projectile speed;
	  */
	 void	SetSpeed(const float& value) { m_speed = value; }

	 /**
	  * Durability setter
	  * @param value: New value for projectile speed;
	  */
	 void	SetTime(const float& value) { m_remainingTime = value; }

	 /**
	  * Damage setter
	  * @param value: New value for projectile damage;
	  */
	 void	SetDamage(const int value) { m_damage = value; }

	 /**
	  * Trigger enter event
	  * @param collider: Collider entering object collider
	  */
	 void	ReachTarget(Core::Physics::Collider* collider);

	 REGISTER_CLASS(ComponentUpdatable)
};

