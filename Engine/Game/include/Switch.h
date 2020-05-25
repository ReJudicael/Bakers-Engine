#pragma once

#include "CoreMinimal.h"
#include "ComponentUpdatable.h"
#include "Collider.h"

// Switch to handle mechanism when player enter triggers
BAKERS_GAME_CLASS Switch : public Core::Datastructure::ComponentBase
{
	Core::Physics::Collider* m_collider = nullptr;
	bool m_switchActivated{ false };

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
	Switch();

	/**
	 * Destructor
	 */
	~Switch();

	/**
	 * Init event
	 */
	void	OnInit() override;

	/**
	 * Activate switch, called by trigger enter
	 * @param collider: Collider of the object entering the trigger
	 */
	void	Activate(Core::Physics::Collider* collider);

	/**
	 * Deactivate switch, called by trigger exit
	 * @param collider: Collider of the object going out of the trigger
	 */
	void	Deactivate(Core::Physics::Collider* collider);

	/**
	 * Indicate whether the switch has been activated or not
	 * @return True if the switch is activated, false otherwise
	 */
	bool	IsActivated() { return m_switchActivated; };

	REGISTER_CLASS(ComponentBase)
};


