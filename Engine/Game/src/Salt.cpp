#include "Salt.h"
#include "AnimationHandler.h"
#include "RootObject.hpp"
#include "EngineCore.h"
#include "LoadResources.h"
#include "SkeletalMesh.h"

RTTR_PLUGIN_REGISTRATION
{
	ZoneScopedN("Registering RTTR")
		ZoneText("Leak happening in this zone is from RTTR and is actually reflexion data. It is a purposeful leak", 98);
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

		.property("Salt Animation", &Salt::m_saltAnimation)
		.property("Boss start", &Salt::m_bossZone);
}


Salt::Salt() : ComponentBase(), AEntity()
{
}

Salt::~Salt()
{
}

void Salt::OnCopy(IComponent* copyTo) const
{
	ComponentBase::OnCopy(copyTo);
	AEntity::OnCopy(copyTo);
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

	m_minions = GetRoot()->GetComponentsOfType<Minion>();

	return ComponentBase::OnStart() && AEntity::OnStart();
}

void Salt::OnDestroy()
{
	ComponentBase::OnDestroy();
	AEntity::OnDestroy();
}

void Salt::OnReset()
{
	ComponentBase::OnReset();
	AEntity::OnReset();

	m_health = 0;
	m_idleAnimation = "";
	m_runAnimation = "";
	m_biteAnimation = "";
	m_getHitAnimation = "";
	m_dieAnimation = "";
	m_saltAnimation = ESaltAnimation::IDLE;
}

void Salt::OnInit()
{
	ComponentBase::OnInit();
	AEntity::OnInit();
}

void Salt::OnUpdate(float deltaTime)
{
	if (!m_hasReachedZone)
	{
		bool allDead = true;
		for (auto it : m_minions)
		{
			if (it)
			{
				if (it->m_health > 0)
				{
					allDead = false;
					break;
				}
			}
		}

		if (allDead)
		{
			m_parent->SetGlobalPos(m_bossZone);
			m_hasReachedZone = true;
		}
	}
}

void Salt::AnimGraph()
{
	Core::Animation::AnimationNode* animFlyIdle{ new Core::Animation::AnimationNode() };
	animFlyIdle->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_flyIdleAnimation);

	Core::Animation::AnimationNode* animLanding{ new Core::Animation::AnimationNode() };
	animLanding->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_landingAnimation);
	animLanding->Loop = false;
	animLanding->speed *= 1.5f;

	Core::Animation::AnimationNode* animIdle{ new Core::Animation::AnimationNode() };
	animIdle->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_idleAnimation);

	Core::Animation::AnimationNode* animRun{ new Core::Animation::AnimationNode() };
	animRun->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_runAnimation);

	Core::Animation::AnimationNode* animBite{ new Core::Animation::AnimationNode() };
	animBite->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_biteAnimation);

	Core::Animation::AnimationNode* animGetHit{ new Core::Animation::AnimationNode() };
	animGetHit->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_getHitAnimation);

	Core::Animation::AnimationNode* animDie{ new Core::Animation::AnimationNode() };
	animDie->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_dieAnimation);
	animDie->Loop = false;

	Core::Animation::TransitionNode* transFlyIdleLanding{ new Core::Animation::TransitionNode() };
	transFlyIdleLanding->InitTransition(animFlyIdle, animLanding, [this] { return m_saltAnimation != ESaltAnimation::FLYIDLE; });

	Core::Animation::TransitionNode* transLandingIdle{ new Core::Animation::TransitionNode() };
	transLandingIdle->InitTransition(animLanding, animIdle);

	Core::Animation::TransitionNode* transIdleRun{ new Core::Animation::TransitionNode() };
	transIdleRun->InitTransition(animIdle, animRun, [this] { return m_saltAnimation == ESaltAnimation::RUN; });
	Core::Animation::TransitionNode* transIdleBite{ new Core::Animation::TransitionNode() };
	transIdleBite->InitTransition(animIdle, animBite, [this] { return m_saltAnimation == ESaltAnimation::BITE; });
	Core::Animation::TransitionNode* transIdleGetHit{ new Core::Animation::TransitionNode() };
	transIdleGetHit->InitTransition(animIdle, animGetHit, [this] { return m_saltAnimation == ESaltAnimation::GETHIT; });
	Core::Animation::TransitionNode* transIdleDie{ new Core::Animation::TransitionNode() };
	transIdleDie->InitTransition(animIdle, animDie, [this] { return m_saltAnimation == ESaltAnimation::DIE; });

	Core::Animation::TransitionNode* transRunIdle{ new Core::Animation::TransitionNode() };
	transRunIdle->InitTransition(animRun, animIdle, [this] { return m_saltAnimation == ESaltAnimation::IDLE; });
	Core::Animation::TransitionNode* transRunBite{ new Core::Animation::TransitionNode() };
	transRunBite->InitTransition(animRun, animBite, [this] { return m_saltAnimation == ESaltAnimation::BITE; });
	Core::Animation::TransitionNode* transRunGetHit{ new Core::Animation::TransitionNode() };
	transRunGetHit->InitTransition(animRun, animGetHit, [this] { return m_saltAnimation == ESaltAnimation::GETHIT; });
	Core::Animation::TransitionNode* transRunDie{ new Core::Animation::TransitionNode() };
	transRunDie->InitTransition(animRun, animDie, [this] { return m_saltAnimation == ESaltAnimation::DIE; });

	Core::Animation::TransitionNode* transBiteIdle{ new Core::Animation::TransitionNode() };
	transBiteIdle->InitTransition(animBite, animIdle, [this] { return m_saltAnimation == ESaltAnimation::IDLE; });
	Core::Animation::TransitionNode* transBiteRun{ new Core::Animation::TransitionNode() };
	transBiteRun->InitTransition(animBite, animRun, [this] { return m_saltAnimation == ESaltAnimation::RUN; });
	Core::Animation::TransitionNode* transBiteGetHit{ new Core::Animation::TransitionNode() };
	transBiteGetHit->InitTransition(animBite, animGetHit, [this] { return m_saltAnimation == ESaltAnimation::GETHIT; });
	Core::Animation::TransitionNode* transBiteDie{ new Core::Animation::TransitionNode() };
	transBiteDie->InitTransition(animBite, animDie, [this] { return m_saltAnimation == ESaltAnimation::DIE; });

	Core::Animation::TransitionNode* transGetHitIdle{ new Core::Animation::TransitionNode() };
	transGetHitIdle->InitTransition(animGetHit, animIdle, [this] { return m_saltAnimation == ESaltAnimation::IDLE; });
	Core::Animation::TransitionNode* transGetHitRun{ new Core::Animation::TransitionNode() };
	transGetHitRun->InitTransition(animGetHit, animRun, [this] { return m_saltAnimation == ESaltAnimation::RUN; });
	Core::Animation::TransitionNode* transGetHitBite{ new Core::Animation::TransitionNode() };
	transGetHitBite->InitTransition(animGetHit, animBite, [this] { return m_saltAnimation == ESaltAnimation::BITE; });
	Core::Animation::TransitionNode* transGetHitDie{ new Core::Animation::TransitionNode() };
	transGetHitDie->InitTransition(animGetHit, animDie, [this] { return m_saltAnimation == ESaltAnimation::DIE; });

	Core::Animation::TransitionNode* transDieFlyIdle{ new Core::Animation::TransitionNode() };
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
		sktmesh->animationHandler = new Core::Animation::AnimationHandler{ animFlyIdle };
	}
}
