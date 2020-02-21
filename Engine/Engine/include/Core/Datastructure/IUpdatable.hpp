#pragma once

#include "IComponent.hpp"

namespace Core::Datastructure
{
	class IUpdatable : public virtual IComponent
	{
	protected:
		bool	m_isUpdating = true;
	public:
		virtual void	OnStart() override;
		virtual void	OnUpdate(float deltaTime) = 0;

		inline void			Update(float deltaTime);
	};

	inline void	IUpdatable::Update(float deltaTime)
	{
		if (m_isUpdating && m_isActive)
			OnUpdate(deltaTime);
	}
}