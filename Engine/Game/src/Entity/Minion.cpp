#include "Minion.h"
#include "AnimationHandler.h"
#include "RootObject.hpp"
#include "EngineCore.h"
#include "ResourcesManager.h"
#include "SkeletalMesh.h"
#include "Owen.h"
#include "Brioche.h"
#include "Collider.h"

RTTR_PLUGIN_REGISTRATION
{
	ZoneScopedN("Registering RTTR")
		ZoneText("Leak happening in this zone is from RTTR and is actually reflexion data. It is a purposeful leak", 98);
	registration::class_<Minion>("Minion")
		.enumeration<EMinionAnimation>("Minion Animation")
		(
			value("Idle", EMinionAnimation::IDLE),
			value("Run", EMinionAnimation::RUN),
			value("Bite", EMinionAnimation::BITE),
			value("Get Hit", EMinionAnimation::GETHIT),
			value("Die", EMinionAnimation::DIE)
		)

		.constructor()
		.property("Idle Animation", &Minion::m_idleAnimation)
		.property("Run Animation", &Minion::m_runAnimation)
		.property("Bite Animation", &Minion::m_biteAnimation)
		.property("Get Hit Animation", &Minion::m_getHitAnimation)
		.property("Die Animation", &Minion::m_dieAnimation)

		.property("Minion Animation", &Minion::m_minionAnimation);
}


Minion::Minion() : ComponentBase(), AEntity()
{
}

Minion::~Minion()
{
}

void Minion::OnCopy(IComponent* copyTo) const
{
	ComponentBase::OnCopy(copyTo);
	AEntity::OnCopy(copyTo);

	Minion* copy{ dynamic_cast<Minion*>(copyTo) };
	copy->m_health = m_health;
	copy->m_damage = m_damage;
	copy->m_idleAnimation = m_idleAnimation;
	copy->m_runAnimation = m_runAnimation;
	copy->m_biteAnimation = m_biteAnimation;
	copy->m_getHitAnimation = m_getHitAnimation;
	copy->m_dieAnimation = m_dieAnimation;
	copy->m_minionAnimation = m_minionAnimation;
}

void Minion::StartCopy(IComponent*& copyTo) const
{
	copyTo = new Minion();
	OnCopy(copyTo);
}

bool Minion::OnStart()
{
	AnimGraph();

	std::list<Core::Navigation::PathFollowingComponent*> components;
	m_parent->GetComponentsOfBaseType<Core::Navigation::PathFollowingComponent>(components);
	if (components.size() > 0)
		m_navigator = *components.begin();

	std::list<Owen*> owen;
	GetRoot()->GetComponentsOfTypeInChilds<Owen>(owen);
	if (owen.size() > 0)
		m_Owen = (*owen.begin())->GetParent();

	std::list<Brioche*> brioche;
	GetRoot()->GetComponentsOfTypeInChilds<Brioche>(brioche);
	if (owen.size() > 0)
		m_Brioche = (*brioche.begin())->GetParent();

	std::list<Core::Datastructure::Object*> childs = m_parent->GetChildren();

	for (auto child = childs.begin(); child != childs.end(); ++child)
	{
		std::list<Core::Physics::Collider*> collider;
		if ((*child)->GetName().find("HitBoxPunch") != std::string::npos)
		{
			(*child)->GetComponentsOfBaseType(collider);
			if (collider.size() > 0)
			{
				(*collider.begin())->OnTriggerEnterEvent.AddListener(BIND_EVENT(Minion::OnEnterCollider));
				colliderPunch = (*collider.begin());
				colliderPunch->SetActivateCollider(false);
			}
		}
	}
	if (m_navigator)
		m_navigator->SetStopMoving(true);

	return 	ComponentBase::OnStart() && AEntity::OnStart();
}

void Minion::OnDestroy()
{
	ComponentBase::OnDestroy();
	AEntity::OnDestroy();
}

void Minion::OnReset()
{
	ComponentBase::OnReset();
	AEntity::OnReset();

	m_health = 0;
	m_idleAnimation = "";
	m_runAnimation = "";
	m_biteAnimation = "";
	m_getHitAnimation = "";
	m_dieAnimation = "";
	m_minionAnimation = EMinionAnimation::IDLE;
}

void Minion::OnInit()
{
	ComponentBase::OnInit();
	AEntity::OnInit();
}

void Minion::OnUpdate(float deltaTime)
{
	if (m_minionAnimation == EMinionAnimation::DIE)
		return;
	if (m_health <= 0)
	{
		m_minionAnimation = EMinionAnimation::DIE;
		return;
	}

	if (m_minionAnimation == EMinionAnimation::GETHIT)
	{
		if (colliderPunch)
			colliderPunch->SetActivateCollider(false);
		m_attackTimer = 0.f;
		return;
	}
	if (colliderPunch)
	{
		if (m_attackTimer >= m_AttackMaxTime)
		{
			m_minionAnimation = EMinionAnimation::IDLE;
			m_attackTimer = 0.f;
			if (colliderPunch && colliderPunch->IsActive())
				colliderPunch->SetActivateCollider(false);
		}

		if (m_minionAnimation == EMinionAnimation::BITE && m_attackTimer >= m_AttackMaxTime - 4.f && !colliderPunch->IsActive())
			colliderPunch->SetActivateCollider(true);
	}

	if (m_minionAnimation != EMinionAnimation::BITE && m_minionAnimation != EMinionAnimation::GETHIT)
	{
		if (m_navigator->IsEndOfThePath())
			m_minionAnimation = EMinionAnimation::RUN;
		else
			m_minionAnimation = EMinionAnimation::IDLE;
	}


	if (m_Owen && m_Brioche)
	{
		Core::Maths::Vec3 OwenPos = m_Owen->GetGlobalPos();
		Core::Maths::Vec3 BriochePos = m_Brioche->GetGlobalPos();
		Core::Maths::Vec3 ParentPos = m_parent->GetGlobalPos();
		float OPLength = (OwenPos - ParentPos).Length();
		float BPLength = (BriochePos - ParentPos).Length();
		if (BPLength < OPLength)
		{
			if (m_currTarget != m_Brioche)
			{
				if (BPLength < 5.f)
					m_currTarget = m_Brioche;
			}
			distToTarget = BPLength;
		}
		else
		{
			if (m_currTarget != m_Owen)
			{
				if (OPLength < 5.f)
					m_currTarget = m_Owen;
			}
			distToTarget = OPLength;
		}
	}
	if (m_currTarget)
	{
		if (m_minionAnimation != EMinionAnimation::BITE && m_navigator)
			m_navigator->SetTarget(m_currTarget->GetGlobalPos());

		if (distToTarget < 1.5f && m_navigator)
		{
			m_minionAnimation = EMinionAnimation::BITE;
			m_navigator->SetStopMoving(true);
		}
		else if (m_navigator)
		{
			m_navigator->SetStopMoving(false);
		}
	}

	if (m_minionAnimation == EMinionAnimation::BITE)
		m_attackTimer += m_AttackSpeed * deltaTime;

}

void Minion::AnimGraph()
{
	Core::Animation::AnimationNode* animIdle{ new Core::Animation::AnimationNode() };
	animIdle->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_idleAnimation);

	Core::Animation::AnimationNode* animRun{ new Core::Animation::AnimationNode() };
	animRun->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_runAnimation);

	Core::Animation::AnimationNode* animBite{ new Core::Animation::AnimationNode() };
	animBite->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_biteAnimation);
	animBite->loop = false;
	m_AttackSpeed = animBite->speed;
	m_AttackMaxTime = animBite->nodeAnimation->Time;

	Core::Animation::AnimationNode* animGetHit{ new Core::Animation::AnimationNode() };
	animGetHit->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_getHitAnimation);
	animGetHit->loop = false;

	Core::Animation::AnimationNode* animDie{ new Core::Animation::AnimationNode() };
	animDie->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_dieAnimation);
	animDie->loop = false;

	Core::Animation::TransitionNode* transIdleRun{ new Core::Animation::TransitionNode() };
	transIdleRun->InitTransition(animIdle, animRun, [this] { return m_minionAnimation == EMinionAnimation::RUN; });
	Core::Animation::TransitionNode* transIdleBite{ new Core::Animation::TransitionNode() };
	transIdleBite->InitTransition(animIdle, animBite, [this] { return m_minionAnimation == EMinionAnimation::BITE; });
	Core::Animation::TransitionNode* transIdleGetHit{ new Core::Animation::TransitionNode() };
	transIdleGetHit->InitTransition(animIdle, animGetHit, [this] { return m_minionAnimation == EMinionAnimation::GETHIT; });
	Core::Animation::TransitionNode* transIdleDie{ new Core::Animation::TransitionNode() };
	transIdleDie->InitTransition(animIdle, animDie, [this] { return m_minionAnimation == EMinionAnimation::DIE; });

	Core::Animation::TransitionNode* transRunIdle{ new Core::Animation::TransitionNode() };
	transRunIdle->InitTransition(animRun, animIdle, [this] { return m_minionAnimation == EMinionAnimation::IDLE; });
	Core::Animation::TransitionNode* transRunBite{ new Core::Animation::TransitionNode() };
	transRunBite->InitTransition(animRun, animBite, [this] { return m_minionAnimation == EMinionAnimation::BITE; });
	Core::Animation::TransitionNode* transRunGetHit{ new Core::Animation::TransitionNode() };
	transRunGetHit->InitTransition(animRun, animGetHit, [this] { return m_minionAnimation == EMinionAnimation::GETHIT; });
	Core::Animation::TransitionNode* transRunDie{ new Core::Animation::TransitionNode() };
	transRunDie->InitTransition(animRun, animDie, [this] { return m_minionAnimation == EMinionAnimation::DIE; });

	Core::Animation::TransitionNode* transBiteIdle{ new Core::Animation::TransitionNode() };
	transBiteIdle->InitTransition(animBite, animIdle);
	Core::Animation::TransitionNode* transBiteRun{ new Core::Animation::TransitionNode() };
	transBiteRun->InitTransition(animBite, animRun, [this] { return m_minionAnimation == EMinionAnimation::RUN; });
	Core::Animation::TransitionNode* transBiteGetHit{ new Core::Animation::TransitionNode() };
	transBiteGetHit->InitTransition(animBite, animGetHit, [this] { return m_minionAnimation == EMinionAnimation::GETHIT; });
	Core::Animation::TransitionNode* transBiteDie{ new Core::Animation::TransitionNode() };
	transBiteDie->InitTransition(animBite, animDie, [this] { return m_minionAnimation == EMinionAnimation::DIE; });

	Core::Animation::TransitionNode* transGetHitIdle{ new Core::Animation::TransitionNode() };
	transGetHitIdle->InitTransition(animGetHit, animIdle, std::bind(&Minion::TransitionGetHitToIdle, this, animGetHit));

	Core::Animation::TransitionNode* transDieIdle{ new Core::Animation::TransitionNode() };
	transDieIdle->InitTransition(animDie, animIdle, [this] { return m_minionAnimation == EMinionAnimation::IDLE && m_health > 0; });

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

	animDie->transitionsAnimation.push_back(transDieIdle);

	std::list<Core::Animation::SkeletalMesh*> skeletalMesh;
	m_parent->GetComponentsOfBaseType<Core::Animation::SkeletalMesh>(skeletalMesh);
	if (skeletalMesh.size() > 0)
	{
		Core::Animation::SkeletalMesh* sktmesh = *skeletalMesh.begin();
		sktmesh->animationHandler = new Core::Animation::AnimationHandler{ animIdle };
	}
}

void Minion::OnEnterCollider(Core::Physics::Collider* collider)
{
	Core::Datastructure::Object* object = collider->GetParent();
	if (object == GetParent())
		return;
	else
	{
		if (object == m_Owen)
			object = m_Owen;
		else if (object == m_Brioche)
			object = m_Brioche;
		else
			return;

		std::list<AEntity*> enemy;
		object->GetComponentsOfBaseType<AEntity>(enemy);

		if (enemy.size() > 0)
		{
			(*enemy.begin())->m_health -= m_damage;
			(*enemy.begin())->IsHit();
		}
	}
}

bool Minion::TransitionGetHitToIdle(Core::Animation::AnimationNode* node)
{
	if (node->DefaultConditionAnimationNode())
	{
		m_minionAnimation = EMinionAnimation::IDLE;
		return true;
	}
	return false;
}

void Minion::IsHit()
{
	m_minionAnimation = EMinionAnimation::GETHIT;
	if(m_navigator)
		m_navigator->SetStopMoving(true);
}
