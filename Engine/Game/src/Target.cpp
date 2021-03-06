#include <imgui\imgui.h>

#include "Target.h"

#include "EngineCore.h"
#include "RootObject.hpp"
#include "BoxCollider.h"
#include "CameraWithSecondaryView.h"

RTTR_PLUGIN_REGISTRATION
{
	ZoneScopedN("Registering RTTR")
		ZoneText("Leak happening in this zone is from RTTR and is actually reflexion data. It is a purposeful leak", 98);
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

	Target* copy = dynamic_cast<Target*>(copyTo);

	copy->m_follower = m_follower;

	copy->m_physicsScenePtr = m_physicsScenePtr;
	copy->m_playerCamera = m_playerCamera;
	copy->m_signal = m_signal;
	copy->m_isLeading = m_isLeading;
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

	std::list<Core::Renderer::Light*> signals;
	m_parent->GetComponentsOfType<Core::Renderer::Light>(signals);
	if (signals.size() > 0)
		m_signal = *signals.begin();

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

	m_follower = nullptr;

	m_physicsScenePtr = nullptr;
	m_playerCamera = nullptr;
	m_signal = nullptr;
	m_isLeading = false;
}

void	Target::FindPlayerCamera()
{
	Owen* player{ nullptr };
	std::list<Owen*> players = GetRoot()->GetComponentsOfType<Owen>();
	if (players.size() > 0)
		player = *players.begin();

	if (!player)
		return;

	std::list<Core::Renderer::CameraWithSecondaryView*> cameras;
	player->GetParent()->GetComponentsOfType<Core::Renderer::CameraWithSecondaryView>(cameras);
	if (cameras.size() > 0)
		m_playerCamera = *cameras.begin();
}

void	Target::SetTargetPosition()
{
	if (Input()->IsKeyDown(EKey::Z))
		m_isLeading = false;

	if (Input()->IsKeyDown(EKey::LEFT_CONTROL) && Input()->IsKeyDown(EKey::SPACE))
		m_follower->SpecialAttack();

	if (Input()->IsKeyDown(EKey::LEFT_CONTROL) && Input()->IsMouseButtonDown(EMouseButton::LEFT))
	{
		m_isLeading = true;

		if (m_signal)
			m_signal->Activate(true);

		Core::Physics::HitResultQuery query;

		Core::Maths::Vec3 origin = m_playerCamera->GetParent()->GetGlobalPos();
		Core::Maths::Vec2 mouse = Input()->GetMousePosInWindow();

		Core::Maths::Vec3 dir = m_playerCamera->GetPerspectiveDirection(mouse.x, mouse.y);
		dir.Normalize();

		physx::PxU32 filter;
		filter |= static_cast<physx::PxU32>(Core::Physics::EFilterRaycast::GROUPE2);
		if (m_physicsScenePtr->Raycast(origin, dir, query, filter))
		{
			m_globalTarget = query.hitPoint;
			if (m_follower->SetEnemy(query.objectHit))
			{
				m_signal->SetAmbient({ 1, 0, 0, 1 });
				m_signal->SetDiffuse({ 1, 0, 0, 1 });
				m_signal->SetSpecular({ 1, 0, 0, 1 });
			}
			else
			{
				m_signal->SetAmbient({ 1, 1, 1, 1 });
				m_signal->SetDiffuse({ 1, 1, 1, 1 });
				m_signal->SetSpecular({ 1, 1, 1, 1 });
			}
			m_newTargetFind = true;
		}
		else
			m_newTargetFind = false;
	}
	else if (!m_isLeading)
	{
		if (m_signal)
			m_signal->Activate(false);
		m_newTargetFind = true;
		m_parent->SetPos({ 0, 0, 0 });
	}
	else
	{
		m_newTargetFind = false;
	}
}

void	Target::WarpBack()
{
	if (m_follower)
	{
		m_isLeading = false;

		if (m_signal)
			m_signal->Activate(false);

		m_parent->SetPos({ 0, 0, 0 });

		m_follower->GetParent()->SetGlobalPos(m_parent->GetGlobalPos());
		m_follower->StopAttack();
	}
}

void	Target::OnInit()
{
	ComponentUpdatable::OnInit();
}

void	Target::OnUpdate(float deltaTime)
{
	SetTargetPosition();

	if (m_isLeading)
		m_parent->SetGlobalPos(m_globalTarget);

	if (m_newTargetFind)
		m_follower->SetTarget(m_parent->GetGlobalPos());

	if (!m_playerCamera || !m_follower || !m_physicsScenePtr)
		return;

	if (Input()->IsKeyDown(EKey::P))
		WarpBack();
}
