#pragma once

#include <list>
#include "Object.hpp"

#include "IUpdatable.hpp"
#include "IRenderable.hpp"
#include "ICamera.h"
#include "InputSystem.hpp"
#include "CoreMinimal.h"
#include "Framebuffer.h"

namespace Core::Datastructure
{
	class EngineCore;

	/**
	 * Root object of the scene, contains all others objects. 
	 * Will start, update, draw and destroy components
	 */
	BAKERS_API_CLASS RootObject : public Object
	{
	protected:
		std::list<IUpdatable*>	m_updatables;
		std::list<IRenderable*>	m_renderables;
		std::list<IComponent*>	m_componentsToStart;
		std::list<ICamera*>		m_cameras;
		std::list<IComponent*>	m_destroyedComponents;
		std::list<Object*>		m_destroyedObjects;

		SystemManagement::InputSystem* m_inputSystem{ nullptr };

		EngineCore* m_engine;
		/**
		 * Default constructor (requires input system)
		 * @param inputSystem: Pointer to Input handling system to link inputs with each objects of hierarchy
		 */
		RootObject(SystemManagement::InputSystem* inputSystem, EngineCore* engine) noexcept;
	public:
		~RootObject()
		{
			if (!IsDestroyed())
				Destroy();
		}
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
		void		Render(std::list<Core::Renderer::Framebuffer*>& fboList) const noexcept;
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
		 * Get the input system stored in the root object
		 * @return Input System
		 */
		SystemManagement::InputSystem* GetInput();

		/**
		 * Get the engine core
		 */
		EngineCore* GetEngine() const noexcept
		{
			return m_engine;
		}

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
		 * Destroys the scene
		 */
		void		Destroy();

		/*
		 * Calls UpdateTransform() of every transform
		 */
		void		UpdateTransforms() { UpdateTransform(); }

		/**
		 * Creates a root node with default transform and no parent,
		 * effectively making it the root node.
		 */
		static RootObject* CreateRootNode(SystemManagement::InputSystem* inputSystem, EngineCore* engine) noexcept;


		/**
		 * Returns all components of the given type stored in the
		 * scene graph. Does not take into account the components stored in the root node
		 */
		template <class Component>
		std::list<Component*> GetComponentsOfType() const noexcept
		{
			std::list<Component*> components;
			GetComponentsOfTypeInChilds(components);
			return components;
		}

		/**
		 * Returns all components of the given base type stored in the
		 * scene graph. Does not take into account the components stored in the root node
		 */
		template <class Component>
		std::list<Component*> GetComponentsOfBaseType() const noexcept
		{
			std::list<Component*> components;
			GetComponentsOfBaseTypeInChilds(components);
			return components;
		}

		REGISTER_CLASS(Object)
	};
}