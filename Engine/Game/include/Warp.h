#pragma once

#include "CoreMinimal.h"
#include "ComponentUpdatable.h"
#include "Vec3.hpp"

BAKERS_GAME_CLASS Warp : public Core::Datastructure::ComponentUpdatable
{
private:
	Core::Maths::Vec3 m_destination;
	float m_countdown{ 0.f };

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

	/**
	 * Call Update function
	 * @param deltaTime: Time elapsed between two frames
	 */
	virtual void OnUpdate(float deltaTime) override;

public:
	/**
	 * Default Constructor
	 */
	Warp();

	/**
	 * Destructor
	 */
	~Warp();

	REGISTER_CLASS(ComponentUpdatable)
};

