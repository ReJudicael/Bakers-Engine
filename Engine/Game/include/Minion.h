#pragma once

#include "CoreMinimal.h"
#include "ComponentUpdatable.h"
#include "AEntity.h"

/**
 * Minion animation
 */
enum class EMinionAnimation : unsigned short
{
	IDLE = 0,
	RUN,
	BITE,
	GETHIT,
	DIE
};

/**
 * Little enemy
 */
BAKERS_GAME_CLASS Minion : public Core::Datastructure::ComponentBase, public virtual AEntity
{
private:
	std::string m_idleAnimation;
	std::string m_runAnimation;
	std::string m_biteAnimation;
	std::string m_getHitAnimation;
	std::string m_dieAnimation;
	EMinionAnimation m_minionAnimation{ EMinionAnimation::IDLE };

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
	Minion();

	/**
	 * Destructor
	 */
	~Minion();

private:
	/**
	 * Set animations and transitions
	 */
	void AnimGraph();

	REGISTER_CLASS(ComponentBase, AEntity)
};
