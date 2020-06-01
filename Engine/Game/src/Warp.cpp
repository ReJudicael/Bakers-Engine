#include "Warp.h"

#include "EngineCore.h"
#include "RootObject.hpp"

RTTR_PLUGIN_REGISTRATION
{
	registration::class_<Warp>("Warp")
		.constructor()
		.property("Destination", &Warp::m_destination)
		.property("Countdown", &Warp::m_countdown);
}


Warp::Warp() : ComponentUpdatable()
{}

Warp::~Warp()
{
}

void	Warp::OnCopy(IComponent* copyTo) const
{
	ZoneScoped
		ComponentUpdatable::OnCopy(copyTo);

	Warp* copy{ dynamic_cast<Warp*>(copyTo) };

	copy->m_destination = m_destination;
	copy->m_countdown = m_countdown;
	copy->m_particles = m_particles;
	copy->m_collider = m_collider;
}

void	Warp::StartCopy(IComponent*& copyTo) const
{
	ZoneScoped
		copyTo = new Warp();
	OnCopy(copyTo);
}

bool	Warp::OnStart()
{
	ZoneScoped

	std::list<Core::Renderer::ParticleSystem*> particles;
	m_parent->GetComponentsOfType<Core::Renderer::ParticleSystem>(particles);
	if (particles.size() > 0)
		m_particles = *particles.begin();
	
	std::list<Core::Physics::BoxCollider*> colliders;
	m_parent->GetComponentsOfTypeInObject<Core::Physics::BoxCollider>(colliders);
	if (colliders.size() > 0)
		m_collider = *colliders.begin();

	std::list<Core::Audio::AudioSource*> audioSources;
	m_parent->GetComponentsOfTypeInObject<Core::Audio::AudioSource>(audioSources);
	if (audioSources.size() > 0)
		m_audioSource = *audioSources.begin();

	if (m_collider)
	{
		m_collider->OnTriggerEnterEvent += BIND_EVENT(Warp::Activate);
		m_collider->OnTriggerExitEvent += BIND_EVENT(Warp::Deactivate);
	}

	return ComponentUpdatable::OnStart();
}

void    Warp::OnDestroy()
{
	ComponentUpdatable::OnDestroy();
}

void    Warp::OnReset()
{
	ComponentUpdatable::OnReset();

	m_canTeleport = false;
	m_countdown = 0.f;
	m_currentTime = 0.f;
	m_destination = { 0.f, 0.f ,0.f };

	m_particles = nullptr;
	m_collider = nullptr;
	m_objectToMove = nullptr;
}

void	Warp::OnInit()
{
	ComponentUpdatable::OnInit();
}

void	Warp::OnUpdate(float deltaTime)
{
	if (!m_collider)
		return;

	if (m_canTeleport)
	{
		m_currentTime -= deltaTime;
		if (m_particles)
			m_particles->Activate(true);

		if (m_currentTime <= 0.f)
		{
			if (m_audioSource)
				m_audioSource->Play();

			m_objectToMove->SetGlobalPos(m_destination);
			m_canTeleport = false;
			if (m_particles)
				m_particles->Activate(false);
		}
	}
	else
	{
		if (m_currentTime != m_countdown)
		{
			m_currentTime = m_countdown;
			if (m_particles)
				m_particles->Activate(false);
		}
	}
}

void	Warp::Activate(Core::Physics::Collider* collider)
{
	m_objectToMove = collider->GetParent();
	m_canTeleport = true;
}

void	Warp::Deactivate(Core::Physics::Collider* collider)
{
	m_canTeleport = false;
	m_objectToMove = nullptr;
}
