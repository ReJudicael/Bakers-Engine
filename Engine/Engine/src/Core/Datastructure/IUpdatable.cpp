#include "IUpdatable.hpp"
#include "RootObject.hpp"
#include "Debug.h"

namespace Core::Datastructure
{
	void Datastructure::IUpdatable::OnStart()
	{
		if (GetScene() == nullptr)
			return;
		GetScene()->AddUpdatable(this);
	}

	void	IUpdatable::Update(float deltaTime)
	{
		ZoneScoped
			ZoneText("Update of a component", 22)
		if (m_isUpdating && m_isActive)
			OnUpdate(deltaTime);
	}
}