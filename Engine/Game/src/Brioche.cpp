#include "Brioche.h"
#include "AnimationHandler.h"
#include "RootObject.hpp"
#include "EngineCore.h"
#include "LoadResources.h"
#include "SkeletalMesh.h"

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

	return ComponentUpdatable::OnStart();
}

void Brioche::OnDestroy()
{
	ComponentUpdatable::OnDestroy();
}

void Brioche::OnReset()
{
	ComponentUpdatable::OnReset();
}

void Brioche::OnInit()
{
	ComponentUpdatable::OnInit();
}

void Brioche::OnUpdate(float deltaTime)
{
}

void Brioche::SetTarget(Core::Maths::Vec3 target)
{
	if (m_navigator)
		m_navigator->SetTarget(target);
}

void Brioche::AnimGraph()
{
	std::shared_ptr<Core::Animation::AnimationNode> animIdle{ std::make_shared<Core::Animation::AnimationNode>() };
	animIdle->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_idleAnimation);

	std::shared_ptr<Core::Animation::AnimationNode> animRun{ std::make_shared<Core::Animation::AnimationNode>() };
	animRun->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_runAnimation);

	std::shared_ptr<Core::Animation::AnimationNode> animBite{ std::make_shared<Core::Animation::AnimationNode>() };
	animBite->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_biteAnimation);

	std::shared_ptr<Core::Animation::AnimationNode> animGetHit{ std::make_shared<Core::Animation::AnimationNode>() };
	animGetHit->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_getHitAnimation);

	std::shared_ptr<Core::Animation::AnimationNode> animDie{ std::make_shared<Core::Animation::AnimationNode>() };
	animDie->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_dieAnimation);
	animDie->Loop = false;

	std::shared_ptr<Core::Animation::TransitionNode> transIdleRun{ std::make_shared<Core::Animation::TransitionNode>() };
	transIdleRun->InitTransition(animIdle, animRun, [this] { return m_briocheAnimation == EBriocheAnimation::RUN; });
	std::shared_ptr<Core::Animation::TransitionNode> transIdleBite{ std::make_shared<Core::Animation::TransitionNode>() };
	transIdleBite->InitTransition(animIdle, animBite, [this] { return m_briocheAnimation == EBriocheAnimation::BITE; });
	std::shared_ptr<Core::Animation::TransitionNode> transIdleGetHit{ std::make_shared<Core::Animation::TransitionNode>() };
	transIdleGetHit->InitTransition(animIdle, animGetHit, [this] { return m_briocheAnimation == EBriocheAnimation::GETHIT; });
	std::shared_ptr<Core::Animation::TransitionNode> transIdleDie{ std::make_shared<Core::Animation::TransitionNode>() };
	transIdleDie->InitTransition(animIdle, animDie, [this] { return m_briocheAnimation == EBriocheAnimation::DIE; });

	std::shared_ptr<Core::Animation::TransitionNode> transRunIdle{ std::make_shared<Core::Animation::TransitionNode>() };
	transRunIdle->InitTransition(animRun, animIdle, [this] { return m_briocheAnimation == EBriocheAnimation::IDLE; });
	std::shared_ptr<Core::Animation::TransitionNode> transRunBite{ std::make_shared<Core::Animation::TransitionNode>() };
	transRunBite->InitTransition(animRun, animBite, [this] { return m_briocheAnimation == EBriocheAnimation::BITE; });
	std::shared_ptr<Core::Animation::TransitionNode> transRunGetHit{ std::make_shared<Core::Animation::TransitionNode>() };
	transRunGetHit->InitTransition(animRun, animGetHit, [this] { return m_briocheAnimation == EBriocheAnimation::GETHIT; });
	std::shared_ptr<Core::Animation::TransitionNode> transRunDie{ std::make_shared<Core::Animation::TransitionNode>() };
	transRunDie->InitTransition(animRun, animDie, [this] { return m_briocheAnimation == EBriocheAnimation::DIE; });

	std::shared_ptr<Core::Animation::TransitionNode> transBiteIdle{ std::make_shared<Core::Animation::TransitionNode>() };
	transBiteIdle->InitTransition(animBite, animIdle, [this] { return m_briocheAnimation == EBriocheAnimation::IDLE; });
	std::shared_ptr<Core::Animation::TransitionNode> transBiteRun{ std::make_shared<Core::Animation::TransitionNode>() };
	transBiteRun->InitTransition(animBite, animRun, [this] { return m_briocheAnimation == EBriocheAnimation::RUN; });
	std::shared_ptr<Core::Animation::TransitionNode> transBiteGetHit{ std::make_shared<Core::Animation::TransitionNode>() };
	transBiteGetHit->InitTransition(animBite, animGetHit, [this] { return m_briocheAnimation == EBriocheAnimation::GETHIT; });
	std::shared_ptr<Core::Animation::TransitionNode> transBiteDie{ std::make_shared<Core::Animation::TransitionNode>() };
	transBiteDie->InitTransition(animBite, animDie, [this] { return m_briocheAnimation == EBriocheAnimation::DIE; });

	std::shared_ptr<Core::Animation::TransitionNode> transGetHitIdle{ std::make_shared<Core::Animation::TransitionNode>() };
	transGetHitIdle->InitTransition(animGetHit, animIdle, [this] { return m_briocheAnimation == EBriocheAnimation::IDLE; });
	std::shared_ptr<Core::Animation::TransitionNode> transGetHitRun{ std::make_shared<Core::Animation::TransitionNode>() };
	transGetHitRun->InitTransition(animGetHit, animRun, [this] { return m_briocheAnimation == EBriocheAnimation::RUN; });
	std::shared_ptr<Core::Animation::TransitionNode> transGetHitBite{ std::make_shared<Core::Animation::TransitionNode>() };
	transGetHitBite->InitTransition(animGetHit, animBite, [this] { return m_briocheAnimation == EBriocheAnimation::BITE; });
	std::shared_ptr<Core::Animation::TransitionNode> transGetHitDie{ std::make_shared<Core::Animation::TransitionNode>() };
	transGetHitDie->InitTransition(animGetHit, animDie, [this] { return m_briocheAnimation == EBriocheAnimation::DIE; });

	std::shared_ptr<Core::Animation::TransitionNode> transDieIdle{ std::make_shared<Core::Animation::TransitionNode>() };
	transDieIdle->InitTransition(animDie, animIdle, [this] { return m_briocheAnimation == EBriocheAnimation::IDLE; });

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
		sktmesh->animationHandler = { animIdle };
	}
}
