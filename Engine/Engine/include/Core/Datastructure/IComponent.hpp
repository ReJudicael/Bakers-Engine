#pragma once

#include "CoreMinimal.h"

namespace Core::SystemManagement
{
	class InputSystem;
}

namespace Core::Datastructure
{
	class Object;
	class RootObject;

	/**
	 * Default component interface. Gives access to OnStart, OnDestroy and
	 * the parent and root node of the component
	 */
	BAKERS_API_CLASS IComponent
	{
	private:
		bool		m_isDestroyed = false;
		bool		m_isStarted = false;
		RootObject* m_root{ nullptr };
	protected:
		bool		m_isActive = true;
		Object*		m_parent{ nullptr };

		/**
		 * Called when the component is created and is assigned to a parent
		 */
		virtual void	OnInit() {};

		/**
		 * Called on start of the component, on the first frame it
		 * is active
		 */
		virtual void	OnStart() {};
		/**
		 * Called when destruction is asked. Destructor will be called
		 * at the end of the frame
		 */
		virtual void	OnDestroy() {};

		/**
		 * Resets the component to its default parameters
		 */
		virtual void	OnReset();

		/**
		 * Copies the data of the component into the given component.
		 * Should always be safe to cast pointer to current component type.
		 */
		virtual void	OnCopy(IComponent * copyTo) const {};
	public:
		/**
		 * Destructor, declared virtual as this class is inherited
		 */
		virtual ~IComponent() noexcept {}

		/**
		 * Initializes component
		 */
		void	Init() noexcept;

		/**
		 * Sets the parent of the component
		 * @param parent: New parent of the component
		 */
		void			SetParent(Object* parent)
		{
			m_parent = parent;
		}

		/**
		 * Sets the root object of the component
		 * @param scene: Root object of the component
		 */
		void			SetScene(RootObject* scene) 
		{
			m_root = scene;
		}

		/**
		 * Gets the parent of the component
		 * @return Parent of the component
		 */
		Object*			GetParent()
		{
			return m_parent;
		}
		/**
		 * Returns root object of the parent
		 * @return Root object
		 */
		RootObject*		GetRoot() const noexcept
		{
			return m_root;
		}

		/**
		 * Returns input system stored in root node
		 * @return Input System pointer from root
		 */
		SystemManagement::InputSystem* Input() noexcept;

		/**
		 * Function called by the engine to start a component.
		 */
		void			Start();

		/**
		 * Marks the component for destruction, calling OnDestroy immediatly
		 * and the destructor afterward
		 */
		void			Destroy();

		/**
		 * Resets the component to its default values. 
		 * It resets the component to after it was added to an object
		 */
		void			Reset();

		/**
		 * Returns if the component has been destroyed
		 * @return State of the component
		 */
		bool			IsDestroyed() const noexcept { return m_isDestroyed; }
		/**
		 * Returns if the component was initialized
		 * @return State of the component
		 */
		bool			IsStarted() const noexcept { return m_isStarted; };
		/**
		 * Returns if the component is active
		 * @return State of the component
		 */
		bool			IsActive() const noexcept { return m_isActive; }
		
		REGISTER_CLASS()
	};
}