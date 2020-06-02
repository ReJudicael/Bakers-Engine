#include "Door.h"

#include "EngineCore.h"
#include "RootObject.hpp"
#include "BoxCollider.h"

RTTR_PLUGIN_REGISTRATION
{
	ZoneScopedN("Registering RTTR")
		ZoneText("Leak happening in this zone is from RTTR and is actually reflexion data. It is a purposeful leak", 98);
	registration::class_<Door>("Door")
		.constructor()
		.property("Movement", &Door::m_movement)
		.property("Speed", &Door::m_speed)
		.property("Time moving", &Door::m_timeMoving);
}


Door::Door() : ComponentUpdatable()
{}

Door::~Door()
{
}

void	Door::OnCopy(IComponent* copyTo) const
{
	ZoneScoped
		ComponentUpdatable::OnCopy(copyTo);

	Door* copy{ dynamic_cast<Door*>(copyTo) };

	copy->m_particles = m_particles;
	copy->m_movement = m_movement;
	copy->m_isMoving = m_isMoving;
	copy->m_speed = m_speed;
	copy->m_timeMoving = m_timeMoving;
	copy->m_currentTime = m_currentTime;
}

void	Door::StartCopy(IComponent*& copyTo) const
{
	ZoneScoped
		copyTo = new Door();
	OnCopy(copyTo);
}

bool	Door::OnStart()
{
	ZoneScoped

	m_switches = GetRoot()->GetComponentsOfType<Switch>();
	
	std::list<Core::Renderer::ParticleSystem*> particles;
	m_parent->GetComponentsOfBaseType<Core::Renderer::ParticleSystem>(particles);
	if (particles.size() > 0)
		m_particles = *particles.begin();

	std::list<Core::Audio::AudioSource*> audioSources;
	m_parent->GetComponentsOfBaseType<Core::Audio::AudioSource>(audioSources);
	if (audioSources.size() > 0)
		m_audioSource = *audioSources.begin();

	if (m_particles)
		m_particles->Activate(false);

	m_currentTime = m_timeMoving;
	m_movement.Normalize();

	return ComponentUpdatable::OnStart();
}

void    Door::OnDestroy()
{
	ComponentUpdatable::OnDestroy();
}

void    Door::OnReset()
{
	ComponentUpdatable::OnReset();

	m_particles = nullptr;
	m_movement = { 0, 0, 1 };
	m_isMoving = false;
	m_speed = 1.f;
	m_timeMoving = 2.f;
	m_currentTime = 0.f;
}

void	Door::OnInit()
{
	ComponentUpdatable::OnInit();
}

void	Door::OnUpdate(float deltaTime)
{
	if (m_isMoving)
	{
		if (m_currentTime <= 0)
		{
			if (m_particles)
				m_particles->Activate(false);
		}
		else
		{
			m_parent->Translate(m_movement * m_speed * deltaTime);
			m_currentTime -= deltaTime;
		}

		return;
	}

	bool allSwitchesActivated = true;
	for (std::list<Switch*>::iterator it{ m_switches.begin() }; it != m_switches.end(); ++it)
	{
		Switch* s = *it;
		if (!s->IsActivated())
		{
			allSwitchesActivated = false;
			break;
		}
	}

	if (allSwitchesActivated)
	{
		m_isMoving = true;
		if (m_particles)
			m_particles->Activate();
		if (m_audioSource)
			m_audioSource->Play();
	}
}
