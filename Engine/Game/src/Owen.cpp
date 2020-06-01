#include "Owen.h"
#include "AnimationHandler.h"
#include "RootObject.hpp"
#include "EngineCore.h"
#include "LoadResources.h"
#include "SkeletalMesh.h"

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


Owen::Owen() : ComponentUpdatable()
{
}

Owen::~Owen()
{
}

void Owen::OnCopy(IComponent* copyTo) const
{
	ComponentUpdatable::OnCopy(copyTo);
	Owen* copy{ dynamic_cast<Owen*>(copyTo) };
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
	return ComponentUpdatable::OnStart();
}

void Owen::OnDestroy()
{
	ComponentUpdatable::OnDestroy();
}

void Owen::OnReset()
{
	ComponentUpdatable::OnReset();
}

void Owen::OnInit()
{
	ComponentUpdatable::OnInit();
}

void Owen::OnUpdate(float deltaTime)
{
	if (m_health <= 0)
	{
		m_owenAnimation = EOwenAnimation::DIE;
		return;
	}

	if (m_rigidbody->GetVelocity().SquaredLength() > 0.1f)
		m_owenAnimation = EOwenAnimation::RUN;
	else
		m_owenAnimation = EOwenAnimation::IDLE;
		

	if (Input()->IsMouseButtonPressed(EMouseButton::LEFT))
	{
		m_owenAnimation = EOwenAnimation::PUNCH;
		m_rigidbody->SetLinearVelocity({ 0.f, m_rigidbody->GetVelocity().y, 0.f });
	}

	if (Input()->IsMouseButtonPressed(EMouseButton::RIGHT))
		m_health = 0;
}

void Owen::AnimGraph()
{
	std::shared_ptr<Core::Animation::AnimationNode> animIdle{ std::make_shared<Core::Animation::AnimationNode>() };
	animIdle->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_idleAnimation);

	std::shared_ptr<Core::Animation::AnimationNode> animRun{ std::make_shared<Core::Animation::AnimationNode>() };
	animRun->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_runAnimation);

	std::shared_ptr<Core::Animation::AnimationNode> animPunch{ std::make_shared<Core::Animation::AnimationNode>() };
	animPunch->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_punchAnimation);
	animPunch->Loop = false;

	std::shared_ptr<Core::Animation::AnimationNode> animGetHit{ std::make_shared<Core::Animation::AnimationNode>() };
	animGetHit->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_getHitAnimation);
	animGetHit->Loop = false;

	std::shared_ptr<Core::Animation::AnimationNode> animDie{ std::make_shared<Core::Animation::AnimationNode>() };
	animDie->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_dieAnimation);
	animDie->Loop = false;

	std::shared_ptr<Core::Animation::TransitionNode> transIdleRun{ std::make_shared<Core::Animation::TransitionNode>() };
	transIdleRun->InitTransition(animIdle, animRun, [this] { return m_owenAnimation == EOwenAnimation::RUN; });
	std::shared_ptr<Core::Animation::TransitionNode> transIdleBite{ std::make_shared<Core::Animation::TransitionNode>() };
	transIdleBite->InitTransition(animIdle, animPunch, [this] { return m_owenAnimation == EOwenAnimation::PUNCH; });
	std::shared_ptr<Core::Animation::TransitionNode> transIdleGetHit{ std::make_shared<Core::Animation::TransitionNode>() };
	transIdleGetHit->InitTransition(animIdle, animGetHit, [this] { return m_owenAnimation == EOwenAnimation::GETHIT; });
	std::shared_ptr<Core::Animation::TransitionNode> transIdleDie{ std::make_shared<Core::Animation::TransitionNode>() };
	transIdleDie->InitTransition(animIdle, animDie, [this] { return m_owenAnimation == EOwenAnimation::DIE; });

	std::shared_ptr<Core::Animation::TransitionNode> transRunIdle{ std::make_shared<Core::Animation::TransitionNode>() };
	transRunIdle->InitTransition(animRun, animIdle, [this]{ return m_owenAnimation == EOwenAnimation::IDLE; });
	std::shared_ptr<Core::Animation::TransitionNode> transRunPunch{ std::make_shared<Core::Animation::TransitionNode>() };
	transRunPunch->InitTransition(animRun, animPunch, [this] { return m_owenAnimation == EOwenAnimation::PUNCH; });
	std::shared_ptr<Core::Animation::TransitionNode> transRunGetHit{ std::make_shared<Core::Animation::TransitionNode>() };
	transRunGetHit->InitTransition(animRun, animGetHit, [this] { return m_owenAnimation == EOwenAnimation::GETHIT; });
	std::shared_ptr<Core::Animation::TransitionNode> transRunDie{ std::make_shared<Core::Animation::TransitionNode>() };
	transRunDie->InitTransition(animRun, animDie, [this] { return m_owenAnimation == EOwenAnimation::DIE; });

	std::shared_ptr<Core::Animation::TransitionNode> transPunchIdle{ std::make_shared<Core::Animation::TransitionNode>() };
	transPunchIdle->InitTransition(animPunch, animIdle);
	std::shared_ptr<Core::Animation::TransitionNode> transPunchRun{ std::make_shared<Core::Animation::TransitionNode>() };
	transPunchRun->InitTransition(animPunch, animRun, [this] { return m_owenAnimation == EOwenAnimation::RUN; });
	std::shared_ptr<Core::Animation::TransitionNode> transPunchGetHit{ std::make_shared<Core::Animation::TransitionNode>() };
	transPunchGetHit->InitTransition(animPunch, animGetHit, [this] { return m_owenAnimation == EOwenAnimation::GETHIT; });
	std::shared_ptr<Core::Animation::TransitionNode> transPunchDie{ std::make_shared<Core::Animation::TransitionNode>() };
	transPunchDie->InitTransition(animPunch, animDie, [this] { return m_owenAnimation == EOwenAnimation::DIE; });

	std::shared_ptr<Core::Animation::TransitionNode> transGetHitIdle{ std::make_shared<Core::Animation::TransitionNode>() };
	transGetHitIdle->InitTransition(animGetHit, animIdle);
	std::shared_ptr<Core::Animation::TransitionNode> transGetHitRun{ std::make_shared<Core::Animation::TransitionNode>() };
	transGetHitRun->InitTransition(animGetHit, animRun, [this] { return m_owenAnimation == EOwenAnimation::RUN; });
	std::shared_ptr<Core::Animation::TransitionNode> transGetHitBite{ std::make_shared<Core::Animation::TransitionNode>() };
	transGetHitBite->InitTransition(animGetHit, animPunch, [this] { return m_owenAnimation == EOwenAnimation::PUNCH; });
	std::shared_ptr<Core::Animation::TransitionNode> transGetHitDie{ std::make_shared<Core::Animation::TransitionNode>() };
	transGetHitDie->InitTransition(animGetHit, animDie, [this] { return m_owenAnimation == EOwenAnimation::DIE; });

	std::shared_ptr<Core::Animation::TransitionNode> transDieIdle{ std::make_shared<Core::Animation::TransitionNode>() };
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
		sktmesh->animationHandler = { animIdle };
	}
}
