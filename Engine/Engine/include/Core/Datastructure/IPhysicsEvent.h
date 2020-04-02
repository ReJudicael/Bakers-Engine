#pragma once
#include "CoreMinimal.h"
#include "EventSystem.hpp"
#include "IPhysics.h"

namespace Core::Datastructure
{
	BAKERS_API_CLASS IPhysicsEvent
	{
	public:
		Core::SystemManagement::EventSystem<Core::Datastructure::IPhysics*>	OnTriggerEnterEvent;
		Core::SystemManagement::EventSystem<Core::Datastructure::IPhysics*>	OnTriggerExitEvent;
		Core::SystemManagement::EventSystem<Core::Datastructure::IPhysics*>	OnContactEvent;
		Core::SystemManagement::EventSystem<double*>	test;

	public:
		IPhysicsEvent() = default;
	};
}

