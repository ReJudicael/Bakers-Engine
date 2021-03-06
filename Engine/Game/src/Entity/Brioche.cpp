#include "Brioche.h"
#include "AnimationHandler.h"
#include "RootObject.hpp"
#include "EngineCore.h"
#include "ResourcesManager.h"
#include "SkeletalMesh.h"
#include "Collider.h"
#include "Minion.h"
#include "Salt.h"
#include "Projectile.h"
#include "SphereCollider.h"

RTTR_PLUGIN_REGISTRATION
{
	ZoneScopedN("Registering RTTR")
		ZoneText("Leak happening in this zone is from RTTR and is actually reflexion data. It is a purposeful leak", 98);
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
		.property("Idle Animation", &Brioche::m_idleAnimation)
		.property("Run Animation", &Brioche::m_runAnimation)
		.property("Bite Animation", &Brioche::m_biteAnimation)
		.property("Get Hit Animation", &Brioche::m_getHitAnimation)
		.property("Die Animation", &Brioche::m_dieAnimation)
	
		.property("Brioche Animation", &Brioche::m_briocheAnimation)

		.property("Fire countdown", &Brioche::m_specialAttackCountDown)
		.property("Fire cooldown", &Brioche::m_specialAttackCoolDown)
		.property("Fire damage", &Brioche::m_specialDamage)
		.property("Fire speed", &Brioche::m_projectileSpeed)
		.property("Fire durability", &Brioche::m_projectileDurability);
}


Brioche::Brioche() : ComponentBase(), AEntity()
{
}

Brioche::~Brioche()
{
}

void Brioche::OnCopy(IComponent* copyTo) const
{
	ComponentBase::OnCopy(copyTo);
	AEntity::OnCopy(copyTo);
	Brioche* copy{ dynamic_cast<Brioche*>(copyTo) };
	copy->m_health = m_health;
	copy->m_idleAnimation = m_idleAnimation;
	copy->m_runAnimation = m_runAnimation;
	copy->m_biteAnimation = m_biteAnimation;
	copy->m_getHitAnimation = m_getHitAnimation;
	copy->m_dieAnimation = m_dieAnimation;
	copy->m_briocheAnimation = m_briocheAnimation;
	copy->m_rigidbody = m_rigidbody;
	copy->m_behavior = m_behavior;
	copy->m_navigator = m_navigator;
	copy->m_specialDamage = m_specialDamage;
	copy->m_specialAttackCoolDown = m_specialAttackCoolDown;
	copy->m_specialAttackCountDown = m_specialAttackCountDown;
	copy->m_projectileSpeed = m_projectileSpeed;
	copy->m_projectileDurability = m_projectileDurability;
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

	std::list<Core::Renderer::ParticleSystem*> particles;
	m_parent->GetComponentsOfType<Core::Renderer::ParticleSystem>(particles);
	if (particles.size() > 0)
		m_fireParticles = *particles.begin();

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
				colliderPunch->SetActivateCollider(false);
			}
		}
	}

	return ComponentBase::OnStart() && AEntity::OnStart();
}

void Brioche::OnDestroy()
{
	ComponentBase::OnDestroy();
	AEntity::OnDestroy();
}

void Brioche::OnReset()
{
	ComponentBase::OnReset();
	AEntity::OnReset();

	m_health = 0;
	m_idleAnimation = "";
	m_runAnimation = "";
	m_biteAnimation = "";
	m_getHitAnimation = "";
	m_dieAnimation = "";
	m_briocheAnimation = EBriocheAnimation::IDLE;

	m_rigidbody = nullptr;
	m_behavior = EBriocheBehavior::FOLLOW;
	m_navigator = nullptr;

	m_specialDamage = 10;
	m_specialAttackTimer = 0;
	m_specialAttackStartTimer = 0;
	m_specialAttackCoolDown = 10.f;
	m_specialAttackCountDown = 0.5f;
	m_projectileSpeed = 2.f;
	m_projectileDurability = 10.f;
}

void Brioche::OnInit()
{
	ComponentBase::OnInit();
	AEntity::OnInit();
}

void Brioche::OnUpdate(float deltaTime)
{
	if (m_briocheAnimation == EBriocheAnimation::DIE)
		return;
	if (m_health <= 0)
	{
		m_briocheAnimation = EBriocheAnimation::DIE;
		return;
	}

	if (m_briocheAnimation == EBriocheAnimation::GETHIT)
	{
		if (colliderPunch)
			colliderPunch->SetActivateCollider(false);
		return;
	}

	if (m_behavior == EBriocheBehavior::GO || m_behavior == EBriocheBehavior::FOLLOW)
	{
		if (m_navigator->IsEndOfThePath())
			m_briocheAnimation = EBriocheAnimation::RUN;
		else
			m_briocheAnimation = EBriocheAnimation::IDLE;
	}

	if (m_enemyToAttack && m_behavior == EBriocheBehavior::GO)
	{
		if ((m_enemyToAttack->GetGlobalPos() - m_parent->GetGlobalPos()).Length() <= m_dist)
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

	if (m_specialAttackTimer > 0.f)
		m_specialAttackTimer -= deltaTime;

	if (m_specialAttackStartTimer > 0.f)
	{
		m_specialAttackStartTimer -= deltaTime;
		if (m_specialAttackStartTimer <= 0.f)
			CastProjectile();
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

bool Brioche::SetEnemy(Core::Datastructure::Object* object)
{
	if (object)
	{
		// Check if the given object is an enemy
		std::list<Minion*> components;
		object->GetComponentsOfType<Minion>(components);

		if (components.size() >= 1)
		{
			m_enemyToAttack = object;
			return true;
		}

		// Check if the given object is a boss enemy
		std::list<Salt*> salt;
		object->GetComponentsOfType<Salt>(salt);

		if (salt.size() >= 1)
		{
			m_enemyToAttack = object;
			return true;
		}
	}

	m_enemyToAttack = nullptr;
	return false;
}

void Brioche::AnimGraph()
{
	ZoneScoped
	Core::Animation::AnimationNode* animIdle{ new Core::Animation::AnimationNode() };
	animIdle->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_idleAnimation);

	Core::Animation::AnimationNode* animRun{ new Core::Animation::AnimationNode() };
	animRun->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_runAnimation);

	Core::Animation::AnimationNode* animBite{ new Core::Animation::AnimationNode() };
	animBite->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_biteAnimation);
	animBite->loop = false;
	m_AttackMaxTime = animBite->nodeAnimation->Time;
	m_AttackSpeed = animBite->speed;

	Core::Animation::AnimationNode* animGetHit{ new Core::Animation::AnimationNode() };
	animGetHit->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_getHitAnimation);
	animGetHit->loop = false;

	Core::Animation::AnimationNode* animDie{ new Core::Animation::AnimationNode() };
	animDie->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_dieAnimation);
	animDie->loop = false;

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
	transBiteIdle->InitTransition(animBite, animIdle, std::bind(&Brioche::TransitionBiteToIdle, this, animBite));
	Core::Animation::TransitionNode* transBiteRun{ new Core::Animation::TransitionNode() };
	transBiteRun->InitTransition(animBite, animRun, [this] { return m_briocheAnimation == EBriocheAnimation::RUN; });
	Core::Animation::TransitionNode* transBiteGetHit{ new Core::Animation::TransitionNode() };
	transBiteGetHit->InitTransition(animBite, animGetHit, [this] { return m_briocheAnimation == EBriocheAnimation::GETHIT; });
	Core::Animation::TransitionNode* transBiteDie{ new Core::Animation::TransitionNode() };
	transBiteDie->InitTransition(animBite, animDie, [this] { return m_briocheAnimation == EBriocheAnimation::DIE; });

	Core::Animation::TransitionNode* transGetHitIdle{ new Core::Animation::TransitionNode() };
	transGetHitIdle->InitTransition(animGetHit, animIdle, std::bind(&Brioche::TransitionGetHitToIdle, this, animGetHit));
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
	Core::Datastructure::Object* object = collider->GetParent();
	if (object == GetParent() || object->GetName().find("Owen") != std::string::npos)
		return;
	else
	{
		std::list<AEntity*> enemy; 
		object->GetComponentsOfBaseType<AEntity>(enemy);

		if (enemy.size() > 0)
			(*enemy.begin())->TakeDamage(m_damage);
	}
}

bool Brioche::TransitionBiteToIdle(Core::Animation::AnimationNode* node)
{
	if (node->DefaultConditionAnimationNode() || m_stopAttack)
	{
		m_attackTimer = 0;
		m_behavior = EBriocheBehavior::GO;
		m_briocheAnimation = EBriocheAnimation::IDLE;
		colliderPunch->SetActivateCollider(false);
		m_navigator->SetStopMoving(false);
		if (m_stopAttack)
			m_enemyToAttack = nullptr;
		return true;
	}
	return false;
}

bool Brioche::TransitionGetHitToIdle(Core::Animation::AnimationNode* node)
{
	if (node->DefaultConditionAnimationNode())
	{
		m_briocheAnimation = EBriocheAnimation::IDLE;
		m_behavior = EBriocheBehavior::GO;
		if (m_navigator)
			m_navigator->SetStopMoving(false);
		return true;
	}
	return false;
}

void Brioche::IsHit()
{
	m_briocheAnimation = EBriocheAnimation::GETHIT;
	if (m_navigator)
		m_navigator->SetStopMoving(true);
}

void Brioche::SpecialAttack()
{
	if (m_specialAttackTimer > 0.f || m_health <= 0)
		return;

	m_stopAttack = false;
	m_briocheAnimation = EBriocheAnimation::BITE;
	m_behavior = EBriocheBehavior::ATTACK;
	m_rigidbody->SetLinearVelocity({ 0.f, m_rigidbody->GetVelocity().y, 0.f });
	m_navigator->SetStopMoving(true);

	m_specialAttackTimer = m_specialAttackCoolDown;
	m_specialAttackStartTimer = m_specialAttackCountDown;
}

void Brioche::CastProjectile()
{
	Core::Datastructure::Object* fire = GetRoot()->CreateChild("Fireball", colliderPunch->GetParent()->GetGlobalPos());
	
	Core::Physics::SphereCollider* s = new Core::Physics::SphereCollider();
	fire->AddComponent(s);
	s->Start();
	s->Trigger(true);

	if (m_fireParticles)
	{
		Core::Renderer::ParticleSystem* p = new Core::Renderer::ParticleSystem(*m_fireParticles);
		fire->AddComponent(p);
		p->Start(); // Component start
		p->Restart(); // Specific particle system activation
	}

	Projectile* proj = new Projectile();
	fire->AddComponent(proj);
	proj->Start();
	proj->SetMovement(m_parent->Forward());
	proj->SetSpeed(m_projectileSpeed);
	proj->SetTime(m_projectileDurability);
	proj->SetDamage(m_specialDamage);

	fire->SetGlobalScale({ 5, 5, 5 });
}
