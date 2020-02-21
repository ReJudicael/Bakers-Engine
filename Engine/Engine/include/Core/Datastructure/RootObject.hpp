#pragma once

#include <list>
#include "Object.hpp"
#include "Renderer.h"

#include "IUpdatable.hpp"
#include "IRenderable.hpp"

namespace Core::Datastructure
{
	class RootObject : public Object
	{
	protected:
		std::list<IUpdatable*>	m_updatables;
		std::list<IRenderable*>	m_renderables;
		std::list<IComponent*>	m_componentsToStart;
		std::list<IComponent*>	m_destroyedComponents;
		std::list<Object*>		m_destroyedObjects;
	public:
		RootObject() noexcept;

		void		StartFrame() noexcept;
		void		Update(float deltaTime) const noexcept;
		void		Render() const noexcept;

		void		AddUpdatable(IUpdatable* i) noexcept;
		void		AddRenderable(IRenderable* i) noexcept;
		void		AddStart(IComponent* i) noexcept;

		void		RemoveUpdatable(IUpdatable* i) noexcept;
		void		RemoveRenderable(IRenderable* i) noexcept;

		void		DestroyComponent(IComponent* i) noexcept;
		void		DestroyObject(Object* i) noexcept;

		void		RemoveDestroyed() noexcept;

		/**
		 * Creates a root node with default transform and no parent,
		 * effectively making it the root node.
		 */
		static RootObject* CreateRootNode() noexcept;
	};
}