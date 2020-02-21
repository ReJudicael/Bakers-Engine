#pragma once

#include "IComponent.hpp"

namespace Core::Datastructure
{
	/**
	 * Component interface to make a component updatable
	 */
	class IUpdatable : public virtual IComponent
	{
	protected:
		bool				m_isUpdating = true;
	public:
		virtual void		OnStart() override;
		/**
		 * Called once per frame before rendering. DeltaTime is
		 * passed as an argument. Must be overriden if the component
		 * is inherited.
		 * @param deltaTime: Time since previous frame
		 */
		virtual void		OnUpdate(float deltaTime) = 0;

		/**
		 * Called by the engine to update the component
		 * @param deltaTime: Time since previous frame
		 */
		inline void			Update(float deltaTime);

		/**
		 * Returns if the component is updated.
		 */
		inline bool			IsUpdating();
	};

	inline void	IUpdatable::Update(float deltaTime)
	{
		if (m_isUpdating && m_isActive)
			OnUpdate(deltaTime);
	}
	inline bool IUpdatable::IsUpdating()
	{
		return m_isUpdating;
	}
}