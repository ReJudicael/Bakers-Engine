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
	if (Input()->IsKeyPressed(EKey::E))
	{
		m_owenAnimation = EOwenAnimation::PUNCH;
		if(colliderPunch)
			colliderPunch->SetActivateCollider(true);
	}

	/*if (m_owenAnimation == EOwenAnimation::PUNCH)
	{
		Core::Datastructure::Transform TRS{ GetParent()->GetUpdatedTransform(), {0.f,1.f,1.f} };
		std::vector<Core::Physics::HitResultQuery> result;
		if (GetParent()->GetScene()->GetEngine()->GetPhysicsScene()->CheckOverlap(Core::Physics::EGeometry::BOX, { 10.f, 10.f, 10.f }, TRS, result))
		{
			BAKERS_LOG_MESSAGE("Hit");
			for (size_t i = 0; i < result.size(); i++)
			{
				if (result[i].objectHit)
					BAKERS_LOG_MESSAGE(result[i].objectHit->GetName());
			}
		}
	}*/

}

void Owen::AnimGraph()
{
	Core::Animation::AnimationNode* animIdle{ new Core::Animation::AnimationNode() };
	animIdle->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_idleAnimation);

	//std::shared_ptr<Core::Animation::AnimationNode> animRun{ std::make_shared<Core::Animation::AnimationNode>() };
	Core::Animation::AnimationNode* animRun{ new Core::Animation::AnimationNode() };
	animRun->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_runAnimation);

	//std::shared_ptr<Core::Animation::AnimationNode> animPunch{ std::make_shared<Core::Animation::AnimationNode>() };
	Core::Animation::AnimationNode* animPunch{ new Core::Animation::AnimationNode() };
	animPunch->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_punchAnimation);

	//std::shared_ptr<Core::Animation::AnimationNode> animGetHit{ std::make_shared<Core::Animation::AnimationNode>() };
	Core::Animation::AnimationNode* animGetHit{ new Core::Animation::AnimationNode() };
	animGetHit->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_getHitAnimation);

	//std::shared_ptr<Core::Animation::AnimationNode> animDie{ std::make_shared<Core::Animation::AnimationNode>() };
	Core::Animation::AnimationNode* animDie{ new Core::Animation::AnimationNode() };
	animDie->nodeAnimation = GetRoot()->GetEngine()->GetResourcesManager()->LoadAsAnAnimation(m_dieAnimation);
	animDie->Loop = false;

	Core::Animation::TransitionNode* transIdleRun{ new Core::Animation::TransitionNode() };
	transIdleRun->InitTransition(animIdle, animRun, [this] { return m_owenAnimation == EOwenAnimation::RUN; });
	//std::shared_ptr<Core::Animation::TransitionNode> transIdleBite{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transIdleBite{ new Core::Animation::TransitionNode() };
	transIdleBite->InitTransition(animIdle, animPunch, [this] { return m_owenAnimation == EOwenAnimation::PUNCH; });
	//std::shared_ptr<Core::Animation::TransitionNode> transIdleGetHit{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transIdleGetHit{ new Core::Animation::TransitionNode() };
	transIdleGetHit->InitTransition(animIdle, animGetHit, [this] { return m_owenAnimation == EOwenAnimation::GETHIT; });
	//std::shared_ptr<Core::Animation::TransitionNode> transIdleDie{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transIdleDie{ new Core::Animation::TransitionNode() };
	transIdleDie->InitTransition(animIdle, animDie, [this] { return m_owenAnimation == EOwenAnimation::DIE; });

	//std::shared_ptr<Core::Animation::TransitionNode> transRunIdle{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transRunIdle{ new Core::Animation::TransitionNode() };
	transRunIdle->InitTransition(animRun, animIdle, [this] { return m_owenAnimation == EOwenAnimation::IDLE; });
	//std::shared_ptr<Core::Animation::TransitionNode> transRunPunch{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transRunPunch{ new Core::Animation::TransitionNode() };
	transRunPunch->InitTransition(animRun, animPunch, [this] { return m_owenAnimation == EOwenAnimation::PUNCH; });
	//std::shared_ptr<Core::Animation::TransitionNode> transRunGetHit{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transRunGetHit{ new Core::Animation::TransitionNode() };
	transRunGetHit->InitTransition(animRun, animGetHit, [this] { return m_owenAnimation == EOwenAnimation::GETHIT; });
	//std::shared_ptr<Core::Animation::TransitionNode> transRunDie{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transRunDie{ new Core::Animation::TransitionNode() };
	transRunDie->InitTransition(animRun, animDie, [this] { return m_owenAnimation == EOwenAnimation::DIE; });

	//std::shared_ptr<Core::Animation::TransitionNode> transPunchIdle{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transPunchIdle{ new Core::Animation::TransitionNode() };
	transPunchIdle->InitTransition(animPunch, animIdle, std::bind(&Owen::TransitionPunch, this, animPunch));
	//std::shared_ptr<Core::Animation::TransitionNode> transPunchRun{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transPunchRun{ new Core::Animation::TransitionNode() };
	transPunchRun->InitTransition(animPunch, animRun, [this] { return m_owenAnimation == EOwenAnimation::RUN; });
	//std::shared_ptr<Core::Animation::TransitionNode> transPunchGetHit{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transPunchGetHit{ new Core::Animation::TransitionNode() };
	transPunchGetHit->InitTransition(animPunch, animGetHit, [this] { return m_owenAnimation == EOwenAnimation::GETHIT; });
	//std::shared_ptr<Core::Animation::TransitionNode> transPunchDie{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transPunchDie{ new Core::Animation::TransitionNode() };
	transPunchDie->InitTransition(animPunch, animDie, [this] { return m_owenAnimation == EOwenAnimation::DIE; });

	//std::shared_ptr<Core::Animation::TransitionNode> transGetHitIdle{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transGetHitIdle{ new Core::Animation::TransitionNode() };
	transGetHitIdle->InitTransition(animGetHit, animIdle, [this] { return m_owenAnimation == EOwenAnimation::IDLE; });
	//std::shared_ptr<Core::Animation::TransitionNode> transGetHitRun{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transGetHitRun{ new Core::Animation::TransitionNode() };
	transGetHitRun->InitTransition(animGetHit, animRun, [this] { return m_owenAnimation == EOwenAnimation::RUN; });
	//std::shared_ptr<Core::Animation::TransitionNode> transGetHitBite{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transGetHitBite{ new Core::Animation::TransitionNode() };
	transGetHitBite->InitTransition(animGetHit, animPunch, [this] { return m_owenAnimation == EOwenAnimation::PUNCH; });
	//std::shared_ptr<Core::Animation::TransitionNode> transGetHitDie{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transGetHitDie{ new Core::Animation::TransitionNode() };
	transGetHitDie->InitTransition(animGetHit, animDie, [this] { return m_owenAnimation == EOwenAnimation::DIE; });
	//std::shared_ptr<Core::Animation::TransitionNode> transDieIdle{ std::make_shared<Core::Animation::TransitionNode>() };
	Core::Animation::TransitionNode* transDieIdle{ new Core::Animation::TransitionNode() };
	transDieIdle->InitTransition(animDie, animIdle, [this] { return m_owenAnimation == EOwenAnimation::IDLE; });

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
	BAKERS_LOG_MESSAGE("coucou");
}

bool Owen::TransitionPunch(Core::Animation::AnimationNode* node)
{
	if (node->DefaultConditionAnimationNode())
	{
		if (colliderPunch)
			colliderPunch->SetActivateCollider(false);
		m_owenAnimation = EOwenAnimation::IDLE;
		return true;
	}
	return false;
}
