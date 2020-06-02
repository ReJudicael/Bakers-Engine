#include "Brioche.h"
#include "AnimationHandler.h"
#include "RootObject.hpp"
#include "EngineCore.h"
#include "LoadResources.h"
#include "SkeletalMesh.h"
#include "Collider.h"
#include "Minion.h"

RTTR_PLUGIN_REGISTRATION
{
	registration::class_<Brioche>("Brioche")
		.enumeration<EBriocheAnimation>("Brioche Animation")
		(
			value("Idle", EBriocheAnimation::IDLE),
			value("Run", EBriocheAnimation::RUN),
			value("Bite", EBriocheAnimation::BITE),
			value("Get Hit", EBriocheAnimation::GETHIT),
			value("Die", EBriocheAnimation::DIE)
		)

		.constructor()
		.property("Health", &Brioche::m_health)
		.property("Idle Animation", &Brioche::m_idleAnimation)
		.property("Run Animation", &Brioche::m_runAnimation)
		.property("Bite Animation", &Brioche::m_biteAnimation)
		.property("Get Hit Animation", &Brioche::m_getHitAnimation)
		.property("Die Animation", &Brioche::m_dieAnimation)
	
		.property("Brioche Animation", &Brioche::m_briocheAnimation);
}


Brioche::Brioche() : ComponentUpdatable()
{
}

Brioche::~Brioche()
{
}

void Brioche::OnCopy(IComponent* copyTo) const
{
	ComponentUpdatable::OnCopy(copyTo);
	Brioche* copy{ dynamic_cast<Brioche*>(copyTo) };
	copy->m_health = m_health;
	copy->m_idleAnimation = m_idleAnimation;
	copy->m_runAnimation = m_runAnimation;
	copy->m_biteAnimation = m_biteAnimation;
	copy->m_getHitAnimation = m_getHitAnimation;
	copy->m_dieAnimation = m_dieAnimation;
	copy->m_briocheAnimation = m_briocheAnimation;
}

void Brioche::StartCopy(IComponent*& copyTo) const
{
	copyTo = new Brioche();
	OnCopy(copyTo);
}

bool Brioche::OnStart()
{
	AnimGraph();

	std::list<Core::Navigation::PathFollowingComponent*> components;
	m_parent->GetComponentsOfBaseType<Core::Navigation::PathFollowingComponent>(components);
	if (components.size() > 0)
		m_navigator = *components.begin();

	std::list<Core::Physics::RigidBody*> rigidBodies;
	m_parent->GetComponentsOfTypeInObject<Core::Physics::RigidBody>(rigidBodies);
	if (rigidBodies.size() > 0)
		m_rigidbody = *rigidBodies.begin();

	std::list<Core::Datastructure::Object*> childs = m_parent->GetChildren();

	for (auto child = childs.begin(); child != childs.end(); ++child)
	{
		std::list<Core::Physics::Collider*> collider;
		if ((*child)->GetName().find("HitBoxPunch") != std::string::npos)
		{
			(*child)->GetComponentsOfBaseType(collider);
			if (collider.size() > 0)
			{
				(*collider.begin())->OnTriggerEnterEvent.AddListener(BIND_EVENT(Brioche::OnEnterCollider));
				colliderPunch = (*collider.begin());
			}
		}
	}

	return ComponentUpdatable::OnStart();
}

void Brioche::OnDestroy()
{
	ComponentUpdatable::OnDestroy();
}

void Brioche::OnReset()
{
	ComponentUpdatable::OnReset();
	m_health = 0.f;
	m_idleAnimation = "";
	m_runAnimation = "";
	m_biteAnimation = "";
	m_getHitAnimation = "";
	m_dieAnimation = "";
	m_briocheAnimation = EBriocheAnimation::IDLE;
}

void Brioche::OnInit()
{
	ComponentUpdatable::OnInit();
}

void Brioche::OnUpdate(float deltaTime)
{
	if (m_health <= 0)
	{
		m_briocheAnimation = EBriocheAnimation::DIE;
		return;
	}

	if (m_behavior == EBriocheBehavior::GO || m_behavior == EBriocheBehavior::FOLLOW)
	{
		if (/*m_rigidbody->GetVelocity().SquaredLength() > 0.001f*/m_navigator->IsEndOfThePath())
			m_briocheAnimation = EBriocheAnimation::RUN;
		else
			m_briocheAnimation = EBriocheAnimation::IDLE;
	}

	if (Input()->IsMouseButtonPressed(EMouseButton::RIGHT))
		m_health = 0;

	if (m_enemyToAttack && m_behavior == EBriocheBehavior::GO)
	{
		if ((m_navigator->GetTarget() - m_parent->GetGlobalPos()).Length() <= 2.f)
		{
			m_stopAttack = false;
			m_briocheAnimation = EBriocheAnimation::BITE;
			m_behavior = EBriocheBehavior::ATTACK;
			m_rigidbody->SetLinearVelocity({ 0.f, m_rigidbody->GetVelocity().y, 0.f });
			m_navigator->SetStopMoving(true);
		}
	}
	if (colliderPunch)
	{
		if (m_briocheAnimation == EBriocheAnimation::BITE && m_attackTimer >= m_AttackMaxTime/2.f && !colliderPunch->IsActive())
		{
			colliderPunch->SetActivateCollider(true);
		}
		else
			m_attackTimer += m_AttackSpeed * deltaTime;
	}

}

void Brioche::SetTarget(Core::Maths::Vec3 target)
{
	if (m_navigator)
	{
		m_navigator->SetTarget(target);
		if (m_behavior != EBriocheBehavior::ATTACK)
		{
			m_navigator->SetStopMoving(false);
			m_behavior = EBriocheBehavior::GO;
		}
	}
}

void Brioche::SetEnemy(Core::Datastructure::Object* object)
{
	if (object)
	{
		std::list<Minion*> components;
		object->GetComponentsOfType<Minion>(components);
		if (components.size() >= 1)
			m_enemyToAttack = object;
		else
			m_enemyToAttack = nullptr;
	}
	else
		m_enemyToAttack = nullptr;
}

void Brioche::AnimGraph()
{
	Core::Animation::AnimationNode* animIdle{ new Core::Animation::AnimationNode() };
	animIdle->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_idleAnimation);

	Core::Animation::AnimationNode* animRun{ new Core::Animation::AnimationNode() };
	animRun->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_runAnimation);

	Core::Animation::AnimationNode* animBite{ new Core::Animation::AnimationNode() };
	animBite->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_biteAnimation);
	animBite->Loop = false;
	m_AttackMaxTime = animBite->nodeAnimation->Time;
	m_AttackSpeed = animBite->speed;

	Core::Animation::AnimationNode* animGetHit{ new Core::Animation::AnimationNode() };
	animGetHit->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_getHitAnimation);
	animGetHit->Loop = false;

	Core::Animation::AnimationNode* animDie{ new Core::Animation::AnimationNode() };
	animDie->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_dieAnimation);
	animDie->Loop = false;

	Core::Animation::TransitionNode* transIdleRun{ new Core::Animation::TransitionNode() };
	transIdleRun->InitTransition(animIdle, animRun, [this] { return m_briocheAnimation == EBriocheAnimation::RUN; });
	Core::Animation::TransitionNode* transIdleBite{ new Core::Animation::TransitionNode() };
	transIdleBite->InitTransition(animIdle, animBite, [this] { return m_briocheAnimation == EBriocheAnimation::BITE; });
	Core::Animation::TransitionNode* transIdleGetHit{ new Core::Animation::TransitionNode() };
	transIdleGetHit->InitTransition(animIdle, animGetHit, [this] { return m_briocheAnimation == EBriocheAnimation::GETHIT; });
	Core::Animation::TransitionNode* transIdleDie{ new Core::Animation::TransitionNode() };
	transIdleDie->InitTransition(animIdle, animDie, [this] { return m_briocheAnimation == EBriocheAnimation::DIE; });

	Core::Animation::TransitionNode* transRunIdle{ new Core::Animation::TransitionNode() };
	transRunIdle->InitTransition(animRun, animIdle, [this] { return m_briocheAnimation == EBriocheAnimation::IDLE; });
	Core::Animation::TransitionNode* transRunBite{ new Core::Animation::TransitionNode() };
	transRunBite->InitTransition(animRun, animBite, [this] { return m_briocheAnimation == EBriocheAnimation::BITE; });
	Core::Animation::TransitionNode* transRunGetHit{ new Core::Animation::TransitionNode() };
	transRunGetHit->InitTransition(animRun, animGetHit, [this] { return m_briocheAnimation == EBriocheAnimation::GETHIT; });
	Core::Animation::TransitionNode* transRunDie{ new Core::Animation::TransitionNode() };
	transRunDie->InitTransition(animRun, animDie, [this] { return m_briocheAnimation == EBriocheAnimation::DIE; });

	Core::Animation::TransitionNode* transBiteIdle{ new Core::Animation::TransitionNode() };
	transBiteIdle->InitTransition(animBite, animIdle, std::bind(&Brioche::TransitionPunchToIdle, this, animBite));
	Core::Animation::TransitionNode* transBiteRun{ new Core::Animation::TransitionNode() };
	transBiteRun->InitTransition(animBite, animRun, [this] { return m_briocheAnimation == EBriocheAnimation::RUN; });
	Core::Animation::TransitionNode* transBiteGetHit{ new Core::Animation::TransitionNode() };
	transBiteGetHit->InitTransition(animBite, animGetHit, [this] { return m_briocheAnimation == EBriocheAnimation::GETHIT; });
	Core::Animation::TransitionNode* transBiteDie{ new Core::Animation::TransitionNode() };
	transBiteDie->InitTransition(animBite, animDie, [this] { return m_briocheAnimation == EBriocheAnimation::DIE; });

	Core::Animation::TransitionNode* transGetHitIdle{ new Core::Animation::TransitionNode() };
	transGetHitIdle->InitTransition(animGetHit, animIdle);
	Core::Animation::TransitionNode* transGetHitRun{ new Core::Animation::TransitionNode() };
	transGetHitRun->InitTransition(animGetHit, animRun, [this] { return m_briocheAnimation == EBriocheAnimation::RUN; });
	Core::Animation::TransitionNode* transGetHitBite{ new Core::Animation::TransitionNode() };
	transGetHitBite->InitTransition(animGetHit, animBite, [this] { return m_briocheAnimation == EBriocheAnimation::BITE; });
	Core::Animation::TransitionNode* transGetHitDie{ new Core::Animation::TransitionNode() };
	transGetHitDie->InitTransition(animGetHit, animDie, [this] { return m_briocheAnimation == EBriocheAnimation::DIE; });

	Core::Animation::TransitionNode* transDieIdle{ new Core::Animation::TransitionNode() };
	transDieIdle->InitTransition(animDie, animIdle, [this] { return m_briocheAnimation == EBriocheAnimation::IDLE && m_health > 0; });

	animIdle->transitionsAnimation.push_back(transIdleRun);
	animIdle->transitionsAnimation.push_back(transIdleBite);
	animIdle->transitionsAnimation.push_back(transIdleGetHit);
	animIdle->transitionsAnimation.push_back(transIdleDie);

	animRun->transitionsAnimation.push_back(transRunIdle);
	animRun->transitionsAnimation.push_back(transRunBite);
	animRun->transitionsAnimation.push_back(transRunGetHit);
	animRun->transitionsAnimation.push_back(transRunDie);

	animBite->transitionsAnimation.push_back(transBiteIdle);
	animBite->transitionsAnimation.push_back(transBiteRun);
	animBite->transitionsAnimation.push_back(transBiteGetHit);
	animBite->transitionsAnimation.push_back(transBiteDie);

	animGetHit->transitionsAnimation.push_back(transGetHitIdle);
	animGetHit->transitionsAnimation.push_back(transGetHitRun);
	animGetHit->transitionsAnimation.push_back(transGetHitBite);
	animGetHit->transitionsAnimation.push_back(transGetHitDie);

	animDie->transitionsAnimation.push_back(transDieIdle);

	std::list<Core::Animation::SkeletalMesh*> skeletalMesh;
	m_parent->GetComponentsOfBaseType<Core::Animation::SkeletalMesh>(skeletalMesh);
	if (skeletalMesh.size() > 0)
	{
		Core::Animation::SkeletalMesh* sktmesh = *skeletalMesh.begin();
		sktmesh->animationHandler = new Core::Animation::AnimationHandler{ animIdle };
	}
}

void Brioche::OnEnterCollider(Core::Physics::Collider* collider)
{
	BAKERS_LOG_MESSAGE("coucou");
}

bool Brioche::TransitionPunchToIdle(Core::Animation::AnimationNode* node)
{
	if (node->DefaultConditionAnimationNode())
	{
		m_attackTimer = 0;
		m_behavior = EBriocheBehavior::GO;
		m_briocheAnimation = EBriocheAnimation::IDLE;
		colliderPunch->SetActivateCollider(false);
		m_navigator->SetStopMoving(false);
		return true;
	}
	return false;
}
