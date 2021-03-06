#pragma once

#include "IComponent.hpp"
#include "CoreMinimal.h"

namespace Core::Datastructure
{
	class Object;

	/**
	 * Default implementation of a base component. Every component should inherit it.
	 * Does absolutly nothing
	 */
	BAKERS_API_CLASS ComponentBase : public virtual IComponent
	{
	protected:
		/**
		 * Copies the component in the given pointer.
		 * On override, should not call other versions of the function.
		 */
		virtual void	StartCopy(IComponent*& copyTo) const;

		/**
		 * Resets the component to its default parameters
		 */
		virtual void	OnReset() override;

	public:
		/**
		 * Component Base
		 */
		ComponentBase() = default;

		/**
		 * Default copy constructor
		 */
		ComponentBase(const ComponentBase& c) = default;

		/**
		 * Default destructeur
		 */
		~ComponentBase() = default;

		/**
		 * Creates a copy of the component.
		 * If component is not added to an object, it should be deleted.
		 */
		ComponentBase* GetCopy() const;

		REGISTER_CLASS(IComponent)
	};
}
