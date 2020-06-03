#pragma once

#include "CoreMinimal.h"
#include "ComponentUpdatable.h"
#include "AEntity.h"
#include "Minion.h"
#include "Brioche.h"
#include "Owen.h"
#include "Vec3.hpp"

/**
 * Salt animation
 */
enum class ESaltAnimation : unsigned short
{
	FLYIDLE = 0,
	LANDING,
	IDLE,
	RUN,
	BITE,
	GETHIT,
	DIE
};

/**
 * Your enemy.
 * Too much salt affects the balance of the bread.
 */
BAKERS_GAME_CLASS Salt : public Core::Datastructure::ComponentBase, public virtual AEntity
{
private:
	std::string m_flyIdleAnimation;
	std::string m_idleAnimation;
	std::string m_landingAnimation;
	std::string m_runAnimation;
	std::string m_biteAnimation;
	std::string m_getHitAnimation;
	std::string m_dieAnimation;
	ESaltAnimation m_saltAnimation{ ESaltAnimation::FLYIDLE };
	std::list<Minion*> m_minions;
	bool m_hasReachedZone{ false };
	Core::Maths::Vec3 m_bossZone;

	float			m_attackTimer;
	float			m_AttackMaxTime;
	float			m_AttackSpeed;


	Brioche* m_Brioche;
	Core::Datastructure::Object* m_Owen;
	Core::Navigation::PathFollowingComponent* m_navigator;
	float distToTarget;

	bool combatBegin{ false };
	Core::Physics::Collider* colliderPunch;

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
	Salt();

	/**
	 * Destructor
	 */
	~Salt();

	/**
	 * Function call as an event for the collision OnTriggerEnter
	 * @param collider: the collider with wich he triggered
	 */
	void OnEnterCollider(Core::Physics::Collider* collider);

	/**
	 * Function call as an event it's the transition between
	 * the Punch animation and Idle
	 * @param node: the AnimationNode Punch
	 */
	bool TransitionBiteToIdle(Core::Animation::AnimationNode* node);

private:
	/**
	 * Set animations and transitions
	 */
	void AnimGraph();

	REGISTER_CLASS(ComponentBase, AEntity)
};

