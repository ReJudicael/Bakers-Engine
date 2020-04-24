#include "TriggeredEvent.h"
#include "Object.hpp"

RTTR_PLUGIN_REGISTRATION
{
	using namespace Core::Datastructure;
	rttr::registration::class_<TriggeredEvent>("Triggered Event")
		.constructor()
		.property("AutoDestroy", &TriggeredEvent::m_autoDestroy)
		.property("ConditionTrigger", &TriggeredEvent::m_conditionTrigger)
		.property("FunctionAssigned", &TriggeredEvent::m_functionsAssigned);
}

namespace Core::Datastructure
{
	void TriggeredEvent::OnCopy(IComponent* copyTo) const
	{
		ComponentBase::OnCopy(copyTo);
		IUpdatable::OnCopy(copyTo);
	}

	void TriggeredEvent::StartCopy(IComponent*& copyTo) const
	{
		copyTo = new TriggeredEvent();
		OnCopy(copyTo);
	}

	void TriggeredEvent::OnReset()
	{
		ComponentBase::OnReset();
		IUpdatable::OnReset();
	}
	void TriggeredEvent::OnStart()
	{
		//m_functionsAssigned = false;
		//m_autoDestroy = true;
		ComponentBase::OnStart();
		IUpdatable::OnStart();
	}

	void TriggeredEvent::SetTriggeredEvent(std::function<bool()> condition, std::function<void()> event, bool autoDestroy)
	{
		SetCondition(condition);
		m_triggerEvent += event;
		m_autoDestroy = autoDestroy;
		m_functionsAssigned = true;
	}

	void TriggeredEvent::SetCondition(std::function<bool()> condition)
	{
		m_conditionTrigger = condition;
	}

	void TriggeredEvent::AddTriggerEvent(std::function<void()> event)
	{
		m_triggerEvent += event;
		m_functionsAssigned = true;
	}

	void TriggeredEvent::OnDestroy()
	{
		m_triggerEvent.RemoveAllListeners();
		m_functionsAssigned = false;
		ComponentBase::OnDestroy();
		IUpdatable::OnDestroy();
	}

	void TriggeredEvent::OnUpdate(float deltaTime)
	{
		if (m_functionsAssigned)
		{
			if (m_conditionTrigger())
			{
				m_triggerEvent.Invoke();
				m_triggerEvent.RemoveAllListeners();
				/*if (m_autoDestroy)
					GetParent()->EraseComponent(this);*/
			}
		}
	}
}