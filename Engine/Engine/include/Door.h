#pragma once

#include "CoreMinimal.h"
#include "ComponentUpdatable.h"
#include "Vec3.hpp"
#include "Switch.h"
#include "ParticleSystem.h"

// Moving object connected to a switch
BAKERS_API_CLASS Door : public Core::Datastructure::ComponentUpdatable
{
	std::list<Switch*> m_switches;
	Core::Renderer::ParticleSystem* m_particles;
	Core::Maths::Vec3 m_movement{ 0, 0, 1 };
	bool m_isMoving{ false };
	float m_speed{ 1.f };
	float m_timeMoving{ 2.f };
	float m_currentTime{ 0.f };

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
	Door();

	/**
	 * Destructor
	 */
	~Door();

	/**
	 * Init event
	 */
	void	OnInit() override;

	/**
	 * Call Update function in Lua script
	 * @param deltaTime: Time elapsed between two frames, sent to Lua Update
	 */
	 virtual void OnUpdate(float deltaTime) override;

	 REGISTER_CLASS(ComponentUpdatable)
};




