#pragma once

#include "CoreMinimal.h"
#include "ComponentUpdatable.h"
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
BAKERS_GAME_CLASS Owen : public Core::Datastructure::ComponentUpdatable
{
private:
	Core::Physics::RigidBody* m_rigidbody;
	int m_health{ 100 };

	std::string m_idleAnimation;
	std::string m_runAnimation;
	std::string m_punchAnimation;
	std::string m_getHitAnimation;
	std::string m_dieAnimation;
	EOwenAnimation m_owenAnimation{ EOwenAnimation::IDLE };
	float			m_attackTimer;
	float			m_AttackMaxTime;
	float			m_AttackSpeed;
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
	Owen();

	/**
	 * Destructor
	 */
	~Owen();

	void OnEnterCollider(Core::Physics::Collider* collider);

	bool TransitionPunch(Core::Animation::AnimationNode* node);

private:
	void AnimGraph();

	REGISTER_CLASS(ComponentUpdatable)
};

