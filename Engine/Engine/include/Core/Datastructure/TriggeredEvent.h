#pragma once

#include "CoreMinimal.h"
#include "ComponentBase.h"
#include "IUpdatable.hpp"
#include "EventSystem.hpp"

namespace Core::Datastructure
{
	BAKERS_API_CLASS TriggeredEvent : public Core::Datastructure::ComponentBase, public virtual Core::Datastructure::IUpdatable
	{
	protected:
		std::function<bool()>							m_conditionTrigger;
		Core::SystemManagement::EventSystem<>			m_triggerEvent;
		bool											m_autoDestroy{ true };
		bool											m_functionsAssigned{ false };

	protected:
		virtual void	OnReset() override;
		virtual void	OnCopy(IComponent* copyTo) const override;
		virtual void	StartCopy(IComponent*& copyTo) const override;

	public:
		TriggeredEvent() = default;

		virtual bool	OnStart() override;
		virtual void	SetTriggeredEvent(std::function<bool()> condition, std::function<void()> eventTrigger, bool autoDestroy = true);
		virtual void	SetCondition(std::function<bool()> condition);
		virtual void	AddTriggerEvent(std::function<void()> eventTrigger);
		virtual void	OnDestroy() override;
		virtual void	OnUpdate(float deltaTime);

		REGISTER_CLASS(Core::Datastructure::ComponentBase, Core::Datastructure::IUpdatable)
	};
}

