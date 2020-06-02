#include "Owen.h"
#include "AnimationHandler.h"
#include "RootObject.hpp"
#include "EngineCore.h"
#include "PhysicsScene.h"
#include "LoadResources.h"
#include "SkeletalMesh.h"
#include "Collider.h"

RTTR_PLUGIN_REGISTRATION
{
	registration::class_<Owen>("Owen")
		.enumeration<EOwenAnimation>("Owen Animation")
		(
			value("Idle", EOwenAnimation::IDLE),
			value("Run", EOwenAnimation::RUN),
			value("Punch", EOwenAnimation::PUNCH),
			value("Get Hit", EOwenAnimation::GETHIT),
			value("Die", EOwenAnimation::DIE)
		)

		.constructor()
		.property("Idle Animation", &Owen::m_idleAnimation)
		.property("Run Animation", &Owen::m_runAnimation)
		.property("Punch Animation", &Owen::m_punchAnimation)
		.property("Get Hit Animation", &Owen::m_getHitAnimation)
		.property("Die Animation", &Owen::m_dieAnimation)

		.property("Owen Animation", &Owen::m_owenAnimation);
}


Owen::Owen() : ComponentBase(), AEntity()
{
}

Owen::~Owen()
{
}

void Owen::OnCopy(IComponent* copyTo) const
{
	ComponentBase::OnCopy(copyTo);
	AEntity::OnCopy(copyTo);

	Owen* copy{ dynamic_cast<Owen*>(copyTo) };
	copy->m_health = m_health;
	copy->m_idleAnimation = m_idleAnimation;
	copy->m_runAnimation = m_runAnimation;
	copy->m_punchAnimation = m_punchAnimation;
	copy->m_getHitAnimation = m_getHitAnimation;
	copy->m_dieAnimation = m_dieAnimation;
	copy->m_owenAnimation = m_owenAnimation;
}

void Owen::StartCopy(IComponent*& copyTo) const
{
	copyTo = new Owen();
	OnCopy(copyTo);
}

bool Owen::OnStart()
{
	std::list<Core::Physics::RigidBody*> rigidBodies;
	m_parent->GetComponentsOfTypeInObject<Core::Physics::RigidBody>(rigidBodies);
	if (rigidBodies.size() > 0)
		m_rigidbody = *rigidBodies.begin();

	AnimGraph();

	std::list<Core::Datastructure::Object*> childs = m_parent->GetChildren();

	for (auto child = childs.begin(); child != childs.end(); ++child)
	{
		std::list<Core::Physics::Collider*> collider;
		if ((*child)->GetName().find("HitBoxPunch") != std::string::npos)
		{
			(*child)->GetComponentsOfBaseType(collider);
			if (collider.size() > 0)
			{
				(*collider.begin())->OnTriggerEnterEvent.AddListener(BIND_EVENT(Owen::OnEnterCollider));
				colliderPunch = (*collider.begin());
			}
		}
	}
	return ComponentBase::OnStart() && AEntity::OnStart();
}

void Owen::OnDestroy()
{
	ComponentBase::OnDestroy();
	AEntity::OnDestroy();
}

void Owen::OnReset()
{
	ComponentBase::OnReset();
	AEntity::OnReset();
	m_health = 0.f;
	m_idleAnimation = "";
	m_runAnimation = "";
	m_punchAnimation = "";
	m_getHitAnimation = "";
	m_dieAnimation = "";
	m_owenAnimation = EOwenAnimation::IDLE;
}

void Owen::OnInit()
{
	ComponentBase::OnInit();
	AEntity::OnInit();
}

void Owen::OnUpdate(float deltaTime)
{
	if (!m_rigidbody)
	{
		BAKERS_LOG_ERROR("Owen doesn't have his rigidbody!");
		return;
	}

	if (m_health <= 0)
	{
		m_owenAnimation = EOwenAnimation::DIE;
		return;
	}
	if (m_attackTimer >= m_AttackMaxTime)
		m_attackTimer = 0.f;

	if (m_rigidbody->GetVelocity().SquaredLength() > 0.1f)
		m_owenAnimation = EOwenAnimation::RUN;
	else if(m_owenAnimation == EOwenAnimation::RUN)
		m_owenAnimation = EOwenAnimation::IDLE;
		

	if (Input()->IsMouseButtonPressed(EMouseButton::LEFT))
	{
		m_owenAnimation = EOwenAnimation::PUNCH;
		m_rigidbody->SetLinearVelocity({ 0.f, m_rigidbody->GetVelocity().y, 0.f });
	}
	if (m_owenAnimation == EOwenAnimation::PUNCH)
		m_attackTimer += m_AttackSpeed * deltaTime;

	if (colliderPunch)
	{
		if (m_owenAnimation == EOwenAnimation::PUNCH && m_attackTimer > 10.f && !colliderPunch->IsActive())
			colliderPunch->SetActivateCollider(true);
		else if (m_owenAnimation != EOwenAnimation::PUNCH && colliderPunch->IsActive())
		{
			m_attackTimer = 0;
			colliderPunch->SetActivateCollider(false);
		}
	}

	if (Input()->IsMouseButtonPressed(EMouseButton::RIGHT))
		m_health = 0;
}

void Owen::AnimGraph()
{
	Core::Animation::AnimationNode* animIdle{ new Core::Animation::AnimationNode() };
	animIdle->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_idleAnimation);

	Core::Animation::AnimationNode* animRun{ new Core::Animation::AnimationNode() };
	animRun->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_runAnimation);

	Core::Animation::AnimationNode* animPunch{ new Core::Animation::AnimationNode() };
	animPunch->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_punchAnimation);
	animPunch->Loop = false;
	m_AttackMaxTime = animPunch->nodeAnimation->Time;
	m_AttackSpeed = animPunch->speed;

	Core::Animation::AnimationNode* animGetHit{ new Core::Animation::AnimationNode() };
	animGetHit->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_getHitAnimation);
	animGetHit->Loop = false;

	Core::Animation::AnimationNode* animDie{ new Core::Animation::AnimationNode() };
	animDie->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_dieAnimation);
	animDie->Loop = false;

	Core::Animation::TransitionNode* transIdleRun{ new Core::Animation::TransitionNode() };
	transIdleRun->InitTransition(animIdle, animRun, [this] { return m_owenAnimation == EOwenAnimation::RUN; });
	Core::Animation::TransitionNode* transIdleBite{ new Core::Animation::TransitionNode() };
	transIdleBite->InitTransition(animIdle, animPunch, [this] { return m_owenAnimation == EOwenAnimation::PUNCH; });
	Core::Animation::TransitionNode* transIdleGetHit{ new Core::Animation::TransitionNode() };
	transIdleGetHit->InitTransition(animIdle, animGetHit, [this] { return m_owenAnimation == EOwenAnimation::GETHIT; });
	Core::Animation::TransitionNode* transIdleDie{ new Core::Animation::TransitionNode() };
	transIdleDie->InitTransition(animIdle, animDie, [this] { return m_owenAnimation == EOwenAnimation::DIE; });

	Core::Animation::TransitionNode* transRunIdle{ new Core::Animation::TransitionNode() };
	transRunIdle->InitTransition(animRun, animIdle, [this]{ return m_owenAnimation == EOwenAnimation::IDLE; });
	Core::Animation::TransitionNode* transRunPunch{ new Core::Animation::TransitionNode() };
	transRunPunch->InitTransition(animRun, animPunch, [this] { return m_owenAnimation == EOwenAnimation::PUNCH; });
	Core::Animation::TransitionNode* transRunGetHit{ new Core::Animation::TransitionNode() };
	transRunGetHit->InitTransition(animRun, animGetHit, [this] { return m_owenAnimation == EOwenAnimation::GETHIT; });
	Core::Animation::TransitionNode* transRunDie{ new Core::Animation::TransitionNode() };
	transRunDie->InitTransition(animRun, animDie, [this] { return m_owenAnimation == EOwenAnimation::DIE; });

	Core::Animation::TransitionNode* transPunchIdle{ new Core::Animation::TransitionNode() };
	transPunchIdle->InitTransition(animPunch, animIdle, std::bind(&Owen::TransitionPunch, this, animPunch));
	Core::Animation::TransitionNode* transPunchRun{ new Core::Animation::TransitionNode() };
	transPunchRun->InitTransition(animPunch, animRun, [this] { return m_owenAnimation == EOwenAnimation::RUN; });
	Core::Animation::TransitionNode* transPunchGetHit{ new Core::Animation::TransitionNode() };
	transPunchGetHit->InitTransition(animPunch, animGetHit, [this] { return m_owenAnimation == EOwenAnimation::GETHIT; });
	Core::Animation::TransitionNode* transPunchDie{ new Core::Animation::TransitionNode() };
	transPunchDie->InitTransition(animPunch, animDie, [this] { return m_owenAnimation == EOwenAnimation::DIE; });

	Core::Animation::TransitionNode* transGetHitIdle{ new Core::Animation::TransitionNode() };
	transGetHitIdle->InitTransition(animGetHit, animIdle);
	Core::Animation::TransitionNode* transGetHitRun{ new Core::Animation::TransitionNode() };
	transGetHitRun->InitTransition(animGetHit, animRun, [this] { return m_owenAnimation == EOwenAnimation::RUN; });
	Core::Animation::TransitionNode* transGetHitBite{ new Core::Animation::TransitionNode() };
	transGetHitBite->InitTransition(animGetHit, animPunch, [this] { return m_owenAnimation == EOwenAnimation::PUNCH; });
	Core::Animation::TransitionNode* transGetHitDie{ new Core::Animation::TransitionNode() };
	transGetHitDie->InitTransition(animGetHit, animDie, [this] { return m_owenAnimation == EOwenAnimation::DIE; });
	Core::Animation::TransitionNode* transDieIdle{ new Core::Animation::TransitionNode() };
	transDieIdle->InitTransition(animDie, animIdle, [this] { return m_owenAnimation == EOwenAnimation::IDLE && m_health > 0; });

	animIdle->transitionsAnimation.push_back(transIdleRun);
	animIdle->transitionsAnimation.push_back(transIdleBite);
	animIdle->transitionsAnimation.push_back(transIdleGetHit);
	animIdle->transitionsAnimation.push_back(transIdleDie);

	animRun->transitionsAnimation.push_back(transRunIdle);
	animRun->transitionsAnimation.push_back(transRunPunch);
	animRun->transitionsAnimation.push_back(transRunGetHit);
	animRun->transitionsAnimation.push_back(transRunDie);

	animPunch->transitionsAnimation.push_back(transPunchIdle);
	animPunch->transitionsAnimation.push_back(transPunchRun);
	animPunch->transitionsAnimation.push_back(transPunchGetHit);
	animPunch->transitionsAnimation.push_back(transPunchDie);

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

void Owen::OnEnterCollider(Core::Physics::Collider* collider)
{
}

bool Owen::TransitionPunch(Core::Animation::AnimationNode* node)
{
	if (node->DefaultConditionAnimationNode())
	{
		m_attackTimer = 0;
		m_owenAnimation = EOwenAnimation::IDLE;
		return true;
	}
	return false;
}
