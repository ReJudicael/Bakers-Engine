#pragma once

#include "InputSystem.hpp"
#include "Debug.h"

namespace Core::Datastructure
{
	class Object;
	class RootObject;

	/**
	 * Default component interface. Gives access to OnStart, OnDestroy and
	 * the parent and root node of the component
	 */
	class IComponent
	{
	private:
		bool		m_isDestroyed = false;
		bool		m_isInit = false;
		RootObject* m_root{ nullptr };
	protected:
		bool		m_isActive = true;
		Object*		m_parent{ nullptr };
	public:
		/**
		 * Destructor, declared virtual as this class is inherited
		 */
		virtual ~IComponent() noexcept {}
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
		void			SetScene(RootObject* scene);

		/**
		 * Returns root object of the parent
		 * @return Root object
		 */
		RootObject*		GetScene() const noexcept
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
		 * Returns if the component has been destroyed
		 * @return State of the component
		 */
		bool			IsDestroyed() const noexcept { return m_isDestroyed; }
		/**
		 * Returns if the component was initialized
		 * @return State of the component
		 */
		bool			IsInit() const noexcept { return m_isInit; };
		/**
		 * Returns if the component is active
		 * @return State of the component
		 */
		bool			IsActive() const noexcept { return m_isActive; }

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
	};
}