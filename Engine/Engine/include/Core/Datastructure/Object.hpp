#pragma once

#include <list>

#include "Transform.hpp"
#include "ComponentBase.h"

namespace Core::Datastructure
{
	/**
	 * Container for all components of the game. It updates its transform and holds
	 * the components.
	 */
	class Object
	{
	private:
		bool						m_isDestroyed = false;
	protected:
		Transform					m_transform;
		Object*						m_parent;
		std::list<Object*>			m_childs;
		std::list<ComponentBase*>	m_components;

		/**
		 * Returns the transform, updates it if needed.
		 * @return Updated transform
		 */
		const Transform&	GetUpdatedTransform() noexcept;

		/**
		 * Constructor of the object. Takes as argument the local position
		 * and its parent. Will update the transform if the parent is not null
		 * @param localPos: Local position of the transform
		 * @param parent: Parent of the object
		 */
		Object(const Transform& localPos, Object* parent) noexcept;
	public:
		/**
		 * Destructor of the object. Destroys all of its children and components.
		 */
		~Object() noexcept
		{

			for (auto it{ m_components.begin() }; it != m_components.end(); ++it)
			{
				if (!(*it)->IsDestroyed())
					(*it)->Destroy();

				delete* it;
			}
			for (auto it{ m_childs.begin() }; it != m_childs.end(); ++it)
			{
				if (!(*it)->IsDestroyed())
					(*it)->Destroy();

				delete *it;
			}
		}

		/**
		 * Sets the object for destruction. Calls OnDestroy on every components
		 * and Destroy on all of its childs
		 */
		void				Destroy() noexcept
		{
			m_isDestroyed = true;

			for (auto it{ m_components.begin() }; it != m_components.end(); ++it)
				(*it)->Destroy();
			for (auto it{ m_childs.begin() }; it != m_childs.end(); ++it)
				(*it)->Destroy();
		}

		/**
		 * Returns if the object was marked for destruction
		 */
		bool				IsDestroyed() const noexcept
		{
			return m_isDestroyed;
		}

		/**
		 * Creates a child with given transform
		 * @param localPos: Local position of the object
		 * @return Pointer to created object
		 */
		Object*				CreateChild(const Transform& localPos) noexcept;
		/**
		 * Returns a pointer to the parent of the object. If the
		 * pointer is nullptr, the object is the root node
		 * @return Pointer to the parent
		 */
		inline Object*		GetParent() const noexcept;

		/**
		 * Creates a root node with default transform and no parent,
		 * effectively making it the root node.
		 */
		static Object*		CreateRootNode() noexcept;

		/**
		 * Translates the object in local space by given vector
		 * @param v: Vector to translate by
		 * @return Const reference to position of the object
		 */
		const Maths::Vec3&	Translate(const Maths::Vec3& v) noexcept
		{
			return m_transform.Translate(v);
		}
		/**
		 * Rotates the object in local space by given vector
		 * @param q: Quaternion to rotate by
		 * @return Const reference to rotaion of the object
		 */
		const Maths::Quat&	Rotate(const Maths::Quat& q) noexcept
		{
			return m_transform.Rotate(q);
		}
		/**
		 * Scales the object in local space by given vector
		 * @param v: Vector to scale by
		 * @return Const reference to scale of the object
		 */
		const Maths::Vec3&	Scale(const Maths::Vec3& v) noexcept
		{
			return m_transform.Scale(v);
		}

		/**
		 * Returns the local position of the object
		 * @return Local position of the object
		 */
		const Maths::Vec3&	GetPos() const noexcept
		{
			return m_transform.GetLocalPos();
		}
		/**
		 * Returns the local rotation of the object
		 * @return Local rotation of the object
		 */
		const Maths::Quat&	GetRot() const noexcept
		{
			return m_transform.GetLocalRot();
		}
		/**
		 * Returns the local scale of the object
		 * @return Local scale of the object
		 */
		const Maths::Vec3&	GetScale() const noexcept
		{
			return m_transform.GetLocalScale();
		}

		/**
		 * Returns the global position of the object
		 * @return Local position of the object
		 */
		const Maths::Vec3&	GetGlobalPos() noexcept
		{
			if (!m_transform.IsGPosUpdated())
				m_transform.UpdatePos(m_parent->GetUpdatedTransform());
			return m_transform.GetGlobalPos();
		}
		/**
		 * Returns the global rotation of the object
		 * @return Local rotation of the object
		 */
		const Maths::Quat&	GetGlobalRot() noexcept
		{
			if (!m_transform.IsGPosUpdated())
				m_transform.UpdatePos(m_parent->GetUpdatedTransform());
			return m_transform.GetGlobalRot();
		}
		/**
		 * Returns the global scale of the object
		 * @return Local scale of the object
		 */
		const Maths::Vec3&	GetGlobalScale() noexcept
		{
			if (!m_transform.IsGPosUpdated())
				m_transform.UpdatePos(m_parent->GetUpdatedTransform());
			return m_transform.GetGlobalScale();
		}
		
		/**
		 * Generates, if needed, local TRS matrix and returns it
		 * @return Local TRS matrix
		 */
		const Maths::Mat4&	GetLocalTRS() noexcept
		{
			return m_transform.GetLocalTrs();
		}

		/**
		 * Generates, if needed, global TRS matrix and returns it
		 * @return Global TRS matrix
		 */
		const Maths::Mat4& GetGlobalTRS() noexcept
		{
			if (!m_transform.IsGPosUpdated())
				m_transform.UpdatePos(m_parent->GetUpdatedTransform());
			return m_transform.GetGlobalTrs();
		}

		/**
		 * Adds given component to the object
		 * @param c: Component to add
		 */
		void	AddComponent(ComponentBase* c) noexcept;
		/**
		 * Removes given component from the object, if it
		 * is part of it. It will not do anything if the component
		 * is not part of it.
		 */
		void	RemoveComponent(ComponentBase* c) noexcept;
		/**
		 * Deletes given component from the object, if it
		 * is part of it. It will not do anything if the component
		 * is not part of it.
		 */
		void	EraseComponent(ComponentBase* c) noexcept;

	};

	inline Object* Datastructure::Object::GetParent() const noexcept
	{
		return m_parent;
	}
}