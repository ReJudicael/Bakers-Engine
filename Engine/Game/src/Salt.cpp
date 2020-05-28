#include "Salt.h"
#include "AnimationHandler.h"
#include "RootObject.hpp"
#include "EngineCore.h"
#include "LoadResources.h"
#include "SkeletalMesh.h"

RTTR_PLUGIN_REGISTRATION
{
	registration::class_<Salt>("Salt")
		.enumeration<ESaltAnimation>("salt Animation")
		(
			value("Fly Idle", ESaltAnimation::FLYIDLE),
			value("Idle", ESaltAnimation::IDLE),
			value("Run", ESaltAnimation::RUN),
			value("Bite", ESaltAnimation::BITE),
			value("Get Hit", ESaltAnimation::GETHIT),
			value("Die", ESaltAnimation::DIE)
		)

		.constructor()
		.property("Fly Idle Animation", &Salt::m_flyIdleAnimation)
		.property("Landing Animation", &Salt::m_landingAnimation)
		.property("Idle Animation", &Salt::m_idleAnimation)
		.property("Run Animation", &Salt::m_runAnimation)
		.property("Bite Animation", &Salt::m_biteAnimation)
		.property("Get Hit Animation", &Salt::m_getHitAnimation)
		.property("Die Animation", &Salt::m_dieAnimation)

		.property("Salt Animation", &Salt::m_saltAnimation);
}


Salt::Salt() : ComponentUpdatable()
{
}

Salt::~Salt()
{
}

void Salt::OnCopy(IComponent* copyTo) const
{
	ComponentUpdatable::OnCopy(copyTo);
	Salt* copy{ dynamic_cast<Salt*>(copyTo) };
}

void Salt::StartCopy(IComponent*& copyTo) const
{
	copyTo = new Salt();
	OnCopy(copyTo);
}

bool Salt::OnStart()
{
	AnimGraph();
	return ComponentUpdatable::OnStart();
}

void Salt::OnDestroy()
{
	ComponentUpdatable::OnDestroy();
}

void Salt::OnReset()
{
	ComponentUpdatable::OnReset();
}

void Salt::OnInit()
{
	ComponentUpdatable::OnInit();
}

void Salt::OnUpdate(float deltaTime)
{
}

void Salt::AnimGraph()
{
	std::shared_ptr<Core::Animation::AnimationNode> animFlyIdle{ std::make_shared<Core::Animation::AnimationNode>() };
	animFlyIdle->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_flyIdleAnimation);

	std::shared_ptr<Core::Animation::AnimationNode> animLanding{ std::make_shared<Core::Animation::AnimationNode>() };
	animLanding->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_landingAnimation);
	animLanding->Loop = false;
	animLanding->speed *= 1.5f;

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

	std::shared_ptr<Core::Animation::TransitionNode> transFlyIdleLanding{ std::make_shared<Core::Animation::TransitionNode>() };
	transFlyIdleLanding->InitTransition(animFlyIdle, animLanding, [this] { return m_saltAnimation != ESaltAnimation::FLYIDLE; });

	std::shared_ptr<Core::Animation::TransitionNode> transLandingIdle{ std::make_shared<Core::Animation::TransitionNode>() };
	transLandingIdle->InitTransition(animLanding, animIdle);
	std::shared_ptr<Core::Animation::TransitionNode> transLandingRun{ std::make_shared<Core::Animation::TransitionNode>() };
	transLandingRun->InitTransition(animLanding, animRun);
	std::shared_ptr<Core::Animation::TransitionNode> transLandingBite{ std::make_shared<Core::Animation::TransitionNode>() };
	transLandingBite->InitTransition(animLanding, animBite);
	std::shared_ptr<Core::Animation::TransitionNode> transLandingGetHit{ std::make_shared<Core::Animation::TransitionNode>() };
	transLandingGetHit->InitTransition(animLanding, animGetHit);
	std::shared_ptr<Core::Animation::TransitionNode> transLandingDie{ std::make_shared<Core::Animation::TransitionNode>() };
	transLandingDie->InitTransition(animLanding, animDie);

	std::shared_ptr<Core::Animation::TransitionNode> transIdleRun{ std::make_shared<Core::Animation::TransitionNode>() };
	transIdleRun->InitTransition(animIdle, animRun, [this] { return m_saltAnimation == ESaltAnimation::RUN; });
	std::shared_ptr<Core::Animation::TransitionNode> transIdleBite{ std::make_shared<Core::Animation::TransitionNode>() };
	transIdleBite->InitTransition(animIdle, animBite, [this] { return m_saltAnimation == ESaltAnimation::BITE; });
	std::shared_ptr<Core::Animation::TransitionNode> transIdleGetHit{ std::make_shared<Core::Animation::TransitionNode>() };
	transIdleGetHit->InitTransition(animIdle, animGetHit, [this] { return m_saltAnimation == ESaltAnimation::GETHIT; });
	std::shared_ptr<Core::Animation::TransitionNode> transIdleDie{ std::make_shared<Core::Animation::TransitionNode>() };
	transIdleDie->InitTransition(animIdle, animDie, [this] { return m_saltAnimation == ESaltAnimation::DIE; });

	std::shared_ptr<Core::Animation::TransitionNode> transRunIdle{ std::make_shared<Core::Animation::TransitionNode>() };
	transRunIdle->InitTransition(animRun, animIdle, [this] { return m_saltAnimation == ESaltAnimation::IDLE; });
	std::shared_ptr<Core::Animation::TransitionNode> transRunBite{ std::make_shared<Core::Animation::TransitionNode>() };
	transRunBite->InitTransition(animRun, animBite, [this] { return m_saltAnimation == ESaltAnimation::BITE; });
	std::shared_ptr<Core::Animation::TransitionNode> transRunGetHit{ std::make_shared<Core::Animation::TransitionNode>() };
	transRunGetHit->InitTransition(animRun, animGetHit, [this] { return m_saltAnimation == ESaltAnimation::GETHIT; });
	std::shared_ptr<Core::Animation::TransitionNode> transRunDie{ std::make_shared<Core::Animation::TransitionNode>() };
	transRunDie->InitTransition(animRun, animDie, [this] { return m_saltAnimation == ESaltAnimation::DIE; });

	std::shared_ptr<Core::Animation::TransitionNode> transBiteIdle{ std::make_shared<Core::Animation::TransitionNode>() };
	transBiteIdle->InitTransition(animBite, animIdle, [this] { return m_saltAnimation == ESaltAnimation::IDLE; });
	std::shared_ptr<Core::Animation::TransitionNode> transBiteRun{ std::make_shared<Core::Animation::TransitionNode>() };
	transBiteRun->InitTransition(animBite, animRun, [this] { return m_saltAnimation == ESaltAnimation::RUN; });
	std::shared_ptr<Core::Animation::TransitionNode> transBiteGetHit{ std::make_shared<Core::Animation::TransitionNode>() };
	transBiteGetHit->InitTransition(animBite, animGetHit, [this] { return m_saltAnimation == ESaltAnimation::GETHIT; });
	std::shared_ptr<Core::Animation::TransitionNode> transBiteDie{ std::make_shared<Core::Animation::TransitionNode>() };
	transBiteDie->InitTransition(animBite, animDie, [this] { return m_saltAnimation == ESaltAnimation::DIE; });

	std::shared_ptr<Core::Animation::TransitionNode> transGetHitIdle{ std::make_shared<Core::Animation::TransitionNode>() };
	transGetHitIdle->InitTransition(animGetHit, animIdle, [this] { return m_saltAnimation == ESaltAnimation::IDLE; });
	std::shared_ptr<Core::Animation::TransitionNode> transGetHitRun{ std::make_shared<Core::Animation::TransitionNode>() };
	transGetHitRun->InitTransition(animGetHit, animRun, [this] { return m_saltAnimation == ESaltAnimation::RUN; });
	std::shared_ptr<Core::Animation::TransitionNode> transGetHitBite{ std::make_shared<Core::Animation::TransitionNode>() };
	transGetHitBite->InitTransition(animGetHit, animBite, [this] { return m_saltAnimation == ESaltAnimation::BITE; });
	std::shared_ptr<Core::Animation::TransitionNode> transGetHitDie{ std::make_shared<Core::Animation::TransitionNode>() };
	transGetHitDie->InitTransition(animGetHit, animDie, [this] { return m_saltAnimation == ESaltAnimation::DIE; });

	std::shared_ptr<Core::Animation::TransitionNode> transDieFlyIdle{ std::make_shared<Core::Animation::TransitionNode>() };
	transDieFlyIdle->InitTransition(animDie, animFlyIdle, [this] { return m_saltAnimation == ESaltAnimation::FLYIDLE; });

	animFlyIdle->transitionsAnimation.push_back(transFlyIdleLanding);

	animLanding->transitionsAnimation.push_back(transLandingIdle);

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

	animDie->transitionsAnimation.push_back(transDieFlyIdle);

	std::list<Core::Animation::SkeletalMesh*> skeletalMesh;
	m_parent->GetComponentsOfBaseType<Core::Animation::SkeletalMesh>(skeletalMesh);
	if (skeletalMesh.size() > 0)
	{
		Core::Animation::SkeletalMesh* sktmesh = *skeletalMesh.begin();
		sktmesh->animationHandler = { animFlyIdle };
	}
}
