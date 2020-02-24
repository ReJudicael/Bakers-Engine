#pragma once

#include <list>
#include "Object.hpp"
#include "Renderer.h"

#include "IUpdatable.hpp"
#include "IRenderable.hpp"
#include "ICamera.h"

namespace Core::Datastructure
{
	/**
	 * Root object of the scene, contains all others objects. 
	 * Will start, update, draw and destroy components
	 */
	class RootObject : public Object
	{
	protected:
		std::list<IUpdatable*>	m_updatables;
		std::list<IRenderable*>	m_renderables;
		std::list<IComponent*>	m_componentsToStart;
		std::list<ICamera*>		m_cameras;
		std::list<IComponent*>	m_destroyedComponents;
		std::list<Object*>		m_destroyedObjects;

		/**
		 * Default constructor
		 */
		RootObject() noexcept;
	public:
		/**
		 * Calls OnStart on every component that requires to be started.
		 * If a component cannot be started in the frame, it will try again
		 * in the next one
		 */
		void		StartFrame() noexcept;
		/**
		 * Calls OnStart on every compatible components that are initialized.
		 * If a component is not initialized, it will not be updated
		 */
		void		Update(float deltaTime) const noexcept;
		/**
		 * Calls OnDraw on every compatible components that are initialized.
		 * If a component is not initialized, it will not be updated
		 */
		void		Render() const noexcept;
		/**
		 * Deletes every destroyed components. 
		 */
		void		RemoveDestroyed() noexcept;

		/**
		 * Adds a component to be updated
		 * @param i: Component to be updated
		 */
		void		AddUpdatable(IUpdatable* i) noexcept;
		/**
		 * Adds a component to be rendered
		 * @param i: Component to be rendered
		 */
		void		AddRenderable(IRenderable* i) noexcept;
		/**
		 * Adds a component to be started
		 * @param i: Component to be started
		 */
		void		AddStart(IComponent* i) noexcept;
		/**
		 * Adds a camera
		 * @param i: Camera to be added
		 */
		void		AddCamera(ICamera* i) noexcept;

		/**
		 * removes a component from the updated ones
		 * @param i: Component to be removed
		 */
		void		RemoveUpdatable(IUpdatable* i) noexcept;
		/**
		 * removes a component from the rendered ones
		 * @param i: Component to be removed
		 */
		void		RemoveRenderable(IRenderable* i) noexcept;
		/**
		 * removes a camera
		 * @param i: Camera to be removed
		 */
		void		RemoveCamera(ICamera* i) noexcept;

		/**
		 * set window ratio for each camera
		 * @param ratio: Ratio to set to each camera for perspective matrix
		 */
		void		SetCamerasRatio(float ratio) noexcept;

		/**
		 * Queues a component to be destroyed
		 */
		void		DestroyComponent(IComponent* i) noexcept;

		/**
		 * Queues an object to be destroyed
		 */
		void		DestroyObject(Object* i) noexcept;

		/**
		 * Creates a root node with default transform and no parent,
		 * effectively making it the root node.
		 */
		static RootObject* CreateRootNode() noexcept;
	};
}