#pragma once

#include "CoreMinimal.h"
#include "ComponentUpdatable.h"
#include "RigidBody.h"
#include "ScriptedComponent.h"
#include "AEntity.h"
#include "ParticleSystem.h"

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
 * Owen animation
 */
enum class EOwenAnimation : unsigned short
{
	IDLE,
	RUN,
	PUNCH,
	GETHIT,
	DIE
};

/**
 * The only knight in the region.
 */
BAKERS_GAME_CLASS Owen : public Core::Datastructure::ComponentBase, public virtual AEntity
{
private:
	Core::Physics::RigidBody* m_rigidbody;

	std::string m_idleAnimation;
	std::string m_runAnimation;
	std::string m_punchAnimation;
	std::string m_getHitAnimation;
	std::string m_dieAnimation;
	EOwenAnimation m_owenAnimation{ EOwenAnimation::IDLE };
	float			m_attackTimer;
	float			m_AttackMaxTime;
	float			m_AttackSpeed;
	Core::Physics::Collider* m_colliderPunch;
	Core::Renderer::ParticleSystem* m_hitParticles;

	Core::Datastructure::ScriptedComponent* m_controller;

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
	Owen();

	/**
	 * Destructor
	 */
	~Owen();

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
	bool TransitionPunch(Core::Animation::AnimationNode* node);

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

