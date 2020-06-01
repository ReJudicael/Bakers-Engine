#include "Minion.h"
#include "AnimationHandler.h"
#include "RootObject.hpp"
#include "EngineCore.h"
#include "LoadResources.h"
#include "SkeletalMesh.h"

RTTR_PLUGIN_REGISTRATION
{
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
		.property("Health", &Minion::m_health)
		.property("Idle Animation", &Minion::m_idleAnimation)
		.property("Run Animation", &Minion::m_runAnimation)
		.property("Bite Animation", &Minion::m_biteAnimation)
		.property("Get Hit Animation", &Minion::m_getHitAnimation)
		.property("Die Animation", &Minion::m_dieAnimation)

		.property("Minion Animation", &Minion::m_minionAnimation);
}


Minion::Minion() : ComponentUpdatable()
{
}

Minion::~Minion()
{
}

void Minion::OnCopy(IComponent* copyTo) const
{
	ComponentUpdatable::OnCopy(copyTo);
	Minion* copy{ dynamic_cast<Minion*>(copyTo) };
	copy->m_health = m_health;
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
	return ComponentUpdatable::OnStart();
}

void Minion::OnDestroy()
{
	ComponentUpdatable::OnDestroy();
}

void Minion::OnReset()
{
	ComponentUpdatable::OnReset();
	m_health = 0.f;
	m_idleAnimation = "";
	m_runAnimation = "";
	m_biteAnimation = "";
	m_getHitAnimation = "";
	m_dieAnimation = "";
	m_minionAnimation = EMinionAnimation::IDLE;
}

void Minion::OnInit()
{
	ComponentUpdatable::OnInit();
}

void Minion::OnUpdate(float deltaTime)
{
}

void Minion::AnimGraph()
{
	//std::shared_ptr<Core::Animation::AnimationNode> animIdle{ std::make_shared<Core::Animation::AnimationNode>() };
	Core::Animation::AnimationNode* animIdle{ new Core::Animation::AnimationNode() };
	animIdle->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_idleAnimation);

	//std::shared_ptr<Core::Animation::AnimationNode> animRun{ std::make_shared<Core::Animation::AnimationNode>() };
	Core::Animation::AnimationNode* animRun{ new Core::Animation::AnimationNode() };
	animRun->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_runAnimation);

	//std::shared_ptr<Core::Animation::AnimationNode> animBite{ std::make_shared<Core::Animation::AnimationNode>() };
	Core::Animation::AnimationNode* animBite{ new Core::Animation::AnimationNode() };
	animBite->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_biteAnimation);
	animBite->Loop = false;

	//std::shared_ptr<Core::Animation::AnimationNode> animGetHit{ std::make_shared<Core::Animation::AnimationNode>() };
	Core::Animation::AnimationNode* animGetHit{ new Core::Animation::AnimationNode() };
	animGetHit->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_getHitAnimation);
	animGetHit->Loop = false;

	//std::shared_ptr<Core::Animation::AnimationNode> animDie{ std::make_shared<Core::Animation::AnimationNode>() };
	Core::Animation::AnimationNode* animDie{ new Core::Animation::AnimationNode() };
	animDie->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_dieAnimation);
	animDie->Loop = false;

	//std::shared_ptr<Core::Animation::TransitionNode> transIdleRun{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transIdleRun{ new Core::Animation::TransitionNode() };
	transIdleRun->InitTransition(animIdle, animRun, [this] { return m_minionAnimation == EMinionAnimation::RUN; });
	//std::shared_ptr<Core::Animation::TransitionNode> transIdleBite{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transIdleBite{ new Core::Animation::TransitionNode() };
	transIdleBite->InitTransition(animIdle, animBite, [this] { return m_minionAnimation == EMinionAnimation::BITE; });
	//std::shared_ptr<Core::Animation::TransitionNode> transIdleGetHit{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transIdleGetHit{ new Core::Animation::TransitionNode() };
	transIdleGetHit->InitTransition(animIdle, animGetHit, [this] { return m_minionAnimation == EMinionAnimation::GETHIT; });
	//std::shared_ptr<Core::Animation::TransitionNode> transIdleDie{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transIdleDie{ new Core::Animation::TransitionNode() };
	transIdleDie->InitTransition(animIdle, animDie, [this] { return m_minionAnimation == EMinionAnimation::DIE; });

	//std::shared_ptr<Core::Animation::TransitionNode> transRunIdle{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transRunIdle{ new Core::Animation::TransitionNode() };
	transRunIdle->InitTransition(animRun, animIdle, [this] { return m_minionAnimation == EMinionAnimation::IDLE; });
	//std::shared_ptr<Core::Animation::TransitionNode> transRunBite{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transRunBite{ new Core::Animation::TransitionNode() };
	transRunBite->InitTransition(animRun, animBite, [this] { return m_minionAnimation == EMinionAnimation::BITE; });
	//std::shared_ptr<Core::Animation::TransitionNode> transRunGetHit{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transRunGetHit{ new Core::Animation::TransitionNode() };
	transRunGetHit->InitTransition(animRun, animGetHit, [this] { return m_minionAnimation == EMinionAnimation::GETHIT; });
	//std::shared_ptr<Core::Animation::TransitionNode> transRunDie{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transRunDie{ new Core::Animation::TransitionNode() };
	transRunDie->InitTransition(animRun, animDie, [this] { return m_minionAnimation == EMinionAnimation::DIE; });

	//std::shared_ptr<Core::Animation::TransitionNode> transBiteIdle{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transBiteIdle{ new Core::Animation::TransitionNode() };
	transBiteIdle->InitTransition(animBite, animIdle);
	//std::shared_ptr<Core::Animation::TransitionNode> transBiteRun{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transBiteRun{ new Core::Animation::TransitionNode() };
	transBiteRun->InitTransition(animBite, animRun, [this] { return m_minionAnimation == EMinionAnimation::RUN; });
	//std::shared_ptr<Core::Animation::TransitionNode> transBiteGetHit{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transBiteGetHit{ new Core::Animation::TransitionNode() };
	transBiteGetHit->InitTransition(animBite, animGetHit, [this] { return m_minionAnimation == EMinionAnimation::GETHIT; });
	//std::shared_ptr<Core::Animation::TransitionNode> transBiteDie{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transBiteDie{ new Core::Animation::TransitionNode() };
	transBiteDie->InitTransition(animBite, animDie, [this] { return m_minionAnimation == EMinionAnimation::DIE; });

	//std::shared_ptr<Core::Animation::TransitionNode> transGetHitIdle{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transGetHitIdle{ new Core::Animation::TransitionNode() };
	transGetHitIdle->InitTransition(animGetHit, animIdle);
	//std::shared_ptr<Core::Animation::TransitionNode> transGetHitRun{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transGetHitRun{ new Core::Animation::TransitionNode() };
	transGetHitRun->InitTransition(animGetHit, animRun, [this] { return m_minionAnimation == EMinionAnimation::RUN; });
	//std::shared_ptr<Core::Animation::TransitionNode> transGetHitBite{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transGetHitBite{ new Core::Animation::TransitionNode() };
	transGetHitBite->InitTransition(animGetHit, animBite, [this] { return m_minionAnimation == EMinionAnimation::BITE; });
	//std::shared_ptr<Core::Animation::TransitionNode> transGetHitDie{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transGetHitDie{ new Core::Animation::TransitionNode() };
	transGetHitDie->InitTransition(animGetHit, animDie, [this] { return m_minionAnimation == EMinionAnimation::DIE; });

	//std::shared_ptr<Core::Animation::TransitionNode> transDieIdle{ std::make_shared<Core::Animation::TransitionNode>() };
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
