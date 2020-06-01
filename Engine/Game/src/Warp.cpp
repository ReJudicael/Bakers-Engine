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
	return ComponentUpdatable::OnStart();
}

void    Warp::OnDestroy()
{
	ComponentUpdatable::OnDestroy();
}

void    Warp::OnReset()
{
	ComponentUpdatable::OnReset();

	m_destination = { 0.f, 0.f, 0.f };
	m_countdown = 0.f;
}

void	Warp::OnInit()
{
	ComponentUpdatable::OnInit();
}

void	Warp::OnUpdate(float deltaTime)
{
}
