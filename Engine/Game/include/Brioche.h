#pragma once

#include "AEntity.h"
#include "PathFollowingComponent.h"
#include "RigidBody.h"


namespace Core
{
	namespace Physics
	{
		class Collider;
	}

	namespace Animation
	{
		class AnimationNode;
	}
}
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
BAKERS_GAME_CLASS Brioche : public Core::Datastructure::ComponentBase, public virtual AEntity
{
private:
	Core::Physics::RigidBody* m_rigidbody;

	std::string m_idleAnimation;
	std::string m_runAnimation;
	std::string m_biteAnimation;
	std::string m_getHitAnimation;
	std::string m_dieAnimation;
	EBriocheAnimation m_briocheAnimation{ EBriocheAnimation::IDLE };
	EBriocheBehavior m_behavior{ EBriocheBehavior::FOLLOW };
	float			m_attackTimer;
	float			m_AttackMaxTime;
	float			m_AttackSpeed;
	bool			m_stopAttack{ false };
	Core::Physics::Collider* colliderPunch;
	Core::Datastructure::Object* m_enemyToAttack{nullptr};
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
	 * Set enemy to reach and attack
	 * @param object: Object used as new enemy
	 */
	void	SetEnemy(Core::Datastructure::Object* object);

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

	/**
	 * Set stopAttack to true
	 */
	void StopAttack() { m_stopAttack = true; }

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


	bool TransitionGetHitToIdle(Core::Animation::AnimationNode* node);

	/**
	 * Call when the Entity is consider as hit
	 */
	virtual void IsHit() override;

private:
	/**
	 * Set animations and transitions
	 */
	void AnimGraph();

	REGISTER_CLASS(ComponentBase, AEntity)
};
