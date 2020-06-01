#pragma once

#include "CoreMinimal.h"
#include "ComponentUpdatable.h"
#include "PathFollowingComponent.h"

/**
 * Brioche animation
 */
enum class EBriocheAnimation : unsigned short
{
	IDLE = 0,
	RUN,
	BITE,
	GETHIT,
	DIE
};

enum class EBriocheBehavior : unsigned short
{
	FOLLOW = 0,
	GO,
	ATTACK
};

/**
 * Your faithful companion.
 * He's not cute but he loves you to the point of sacrificing himself.
 */
BAKERS_GAME_CLASS Brioche : public Core::Datastructure::ComponentUpdatable
{
private:
	std::string m_idleAnimation;
	std::string m_runAnimation;
	std::string m_biteAnimation;
	std::string m_getHitAnimation;
	std::string m_dieAnimation;
	EBriocheAnimation m_briocheAnimation{ EBriocheAnimation::IDLE };
	EBriocheBehavior m_behavior{ EBriocheBehavior::FOLLOW };

	Core::Navigation::PathFollowingComponent* m_navigator;

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
	Brioche();

	/**
	 * Destructor
	 */
	~Brioche();

	/**
	 * Set target of path following component
	 * @param target: New position to reach
	 */
	void	SetTarget(Core::Maths::Vec3 target);

	/**
	 * Behavior setter
	 * @param newBehavior: new behavior for ally, can be FOLLOW, GO or ATTACK
	 */
	void	SetBehavior(EBriocheBehavior newBehavior) { m_behavior = newBehavior; };

	/**
	 * Behavior getter
	 * @return Current behavior for ally
	 */
	EBriocheBehavior GetBehavior() { return m_behavior; };

private:
	void AnimGraph();

	REGISTER_CLASS(ComponentUpdatable)
};
