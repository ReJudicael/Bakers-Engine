#pragma once

#include "CoreMinimal.h"
#include "ComponentBase.h"
#include "BoxCollider.h"

BAKERS_GAME_CLASS Deadzone : public Core::Datastructure::ComponentBase
{
private:
	Core::Physics::BoxCollider* m_collider = nullptr;

protected:
	/**
	 * Init event
	 */
	virtual void OnInit() override;


	/**
	 * Copies the data of the component into the given component.
	 * Should always be safe to cast pointer to current component type.
	 */
	virtual void OnCopy(IComponent* copyTo) const override;

	/**
	 * Copies the component in the given pointer.
	 * On override, should not call other versions of the function.
	 */
	virtual void StartCopy(IComponent*& copyTo) const override;

	/**
	 * First frame upon creation event
	 */
	virtual bool OnStart() override;

	/**
	 * Destroy event
	 */
	virtual void OnDestroy() override;

	/**
	 * Reset event
	 */
	virtual void OnReset() override;

public:
	/**
	 * Default Constructor
	 */
	Deadzone();

	/**
	 * Destructor
	 */
	~Deadzone();

private:
	void Activate(Core::Physics::Collider* collider);

	void Deactivate(Core::Physics::Collider* collider);

	REGISTER_CLASS(ComponentBase)
};

