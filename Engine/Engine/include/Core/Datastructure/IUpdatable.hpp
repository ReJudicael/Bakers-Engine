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

		virtual void		OnCopy(void* copyTo) const override;
		virtual void		OnDestroy() override;
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