#pragma once

#include "IComponent.hpp"

namespace Core::Datastructure
{
	class IUpdatable : public IComponent
	{
	protected:
		bool	m_isUpdating = true;
	public:
		virtual void	OnUpdate(float deltaTime) = 0;

		inline void			Update(float deltaTime);
	};


}