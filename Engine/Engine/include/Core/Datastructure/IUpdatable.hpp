#pragma once

#include "IComponent.hpp"
#include "CoreMinimal.h"

namespace Core::Datastructure
{
	/**
	 * Component interface to make a component updatable
	 */
	BAKERS_API_CLASS IUpdatable : public virtual IComponent
	{
	protected:
		bool				m_isUpdating = true; 
		
		virtual void		OnStart() override;
		/**
		 * Called once per frame before rendering. DeltaTime is
		 * passed as an argument. Must be overriden if the component
		 * is inherited.
		 * @param deltaTime: Time since previous frame
		 */
		virtual void		OnUpdate(float deltaTime) = 0;

		/**
		 * Copies the data of the component into the given component.
		 * Should always be safe to cast pointer to current component type.
		 */
		virtual void		OnCopy(IComponent* copyTo) const override;
		virtual void		OnDestroy() override;
		virtual void		OnReset() override;
	public:
		/**
		 * Destructor of the updatable interface
		 */
		~IUpdatable() noexcept;

		/**
		 * Called by the engine to update the component
		 * @param deltaTime: Time since previous frame
		 */
		void				Update(float deltaTime);

		/**
		 * Returns if the component is updated.
		 */
		inline bool			IsUpdating();

		REGISTER_CLASS(IComponent)
	};
	inline bool IUpdatable::IsUpdating()
	{
		return m_isUpdating;
	}
}