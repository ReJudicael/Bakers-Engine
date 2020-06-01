#include <imgui\imgui.h>

#include "Target.h"

#include "EngineCore.h"
#include "RootObject.hpp"
#include "BoxCollider.h"

RTTR_PLUGIN_REGISTRATION
{
	registration::class_<Target>("Target")
		.constructor();
}


Target::Target() : ComponentUpdatable()
{}

Target::~Target()
{
}

void	Target::OnCopy(IComponent* copyTo) const
{
	ZoneScoped
		ComponentUpdatable::OnCopy(copyTo);
}

void	Target::StartCopy(IComponent*& copyTo) const
{
	ZoneScoped
		copyTo = new Target();
	OnCopy(copyTo);
}

bool	Target::OnStart()
{
	ZoneScoped

		m_parent->SetGlobalPos({ 0, 0, 0 });

	std::list<Brioche*> components = GetRoot()->GetComponentsOfType<Brioche>();
	if (components.size() > 0)
		m_follower = *components.begin();

	m_physicsScenePtr = GetRoot()->GetEngine()->GetPhysicsScene();

	FindPlayerCamera();

	return ComponentUpdatable::OnStart();
}

void    Target::OnDestroy()
{
	ComponentUpdatable::OnDestroy();
}

void    Target::OnReset()
{
	ComponentUpdatable::OnReset();

	
}

void	Target::FindPlayerCamera()
{
	Owen* player{ nullptr };
	std::list<Owen*> players = GetRoot()->GetComponentsOfType<Owen>();
	if (players.size() > 0)
		player = *players.begin();

	if (!player)
		return;

	std::list<Core::Renderer::Camera*> cameras;
	player->GetParent()->GetComponentsOfType<Core::Renderer::Camera>(cameras);
	if (cameras.size() > 0)
		m_playerCamera = *cameras.begin();
}

void	Target::SetTargetPosition()
{
	if (Input()->IsKeyDown(EKey::LEFT_SHIFT) && Input()->IsMouseButtonDown(EMouseButton::LEFT))
	{
		Core::Physics::HitResultQuery query;

		Core::Maths::Vec3 origin = m_playerCamera->GetParent()->GetGlobalPos();
		Core::Maths::Vec3 forward = m_playerCamera->GetParent()->Forward() * -1;
		forward.Normalize();

		if (m_physicsScenePtr->Raycast(origin, forward, query))
			m_parent->SetGlobalPos(query.hitPoint);
	}
	else
		m_parent->SetPos({ 0, 0, 0 });
}

void	Target::OnInit()
{
	ComponentUpdatable::OnInit();
}

void	Target::OnUpdate(float deltaTime)
{
	
	if (m_follower->GetBehavior() != EBriocheBehavior::ATTACK)
		m_follower->SetTarget(m_parent->GetGlobalPos());

	if (!m_playerCamera || !m_follower || !m_physicsScenePtr)
		return;

	SetTargetPosition();
}
