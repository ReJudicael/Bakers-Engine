#pragma once

#include <list>

#include "Transform.hpp"
#include "ComponentBase.h"
#include "EventSystem.hpp"
#include "CoreMinimal.h"

namespace Core::Datastructure
{
	class RootObject;

	enum class ObjectFlags : unsigned char
	{
		None = 0,
		STATIC = 1, // Physically static object, will be used in navmesh
		DYNAMICALLY_GENERATED = 1 << 1, // Object generated at runtime, will not be saved into a scene but parameters will be copied after loading
	};

	RTTR_DECLARE_FLAGS(ObjectFlag, ObjectFlags)
	RTTR_DECLARE_ENUM_FLAGS_OPERATORS(ObjectFlag)

	/**
	 * Container for all components of the game. It updates its transform and holds
	 * the components.
	 */
	BAKERS_API_CLASS Object
	{
	private:
		RootObject*								m_root{ nullptr };
		bool									m_isDestroyed = false;
		
	protected:
		bool									m_isActive = true;
		bool									m_isTransformUpdated = true;
		ObjectFlag								m_flags;
		std::string								m_name;
		Transform								m_transform;
		Object* m_parent;
		std::list<Object*>						m_childs;
		std::list<ComponentBase*>				m_components;

		Core::SystemManagement::EventSystem<>	m_EventTransformChange;

		/**
		 * Set own transform plus childs to require an update to position
		 */
		void				RequireUpdate() noexcept
		{
			m_isTransformUpdated = true;
			for (auto it{ m_childs.begin() }; it != m_childs.end(); ++it)
				(*it)->RequireUpdate();
			m_transform.RequireUpdate();
			m_EventTransformChange.Invoke();
		}

		/**
		 * Sets destruction flag to true
		 */
		void				SetDestroyed() noexcept
		{
			m_isDestroyed = true;
		}

		/**
		 * Sets the transform updated bool to false at the end of the frame.
		 * Will not actually update the transform
		 */
		void				UpdateTransform();

		/**
		 * Constructor of the object. Takes as argument the local position
		 * and its parent. Will update the transform if the parent is not null
		 * @param localPos: Local position of the transform
		 * @param parent: Parent of the object
		 * @param Scene: Root node of the object
		 */
		Object(const std::string& name, const Transform& localPos, Object* parent, RootObject* scene) noexcept;
	public:
		/**
		 * Returns the flags of the object
		 */
		ObjectFlag			GetFlags() const noexcept { return m_flags; }

		/**
		 * Returns the transform, updates it if needed.
		 * @return Updated transform
		 */
		const Transform&	GetUpdatedTransform() noexcept;
		/**
		 * Returns if the transform has been updated in the frame
		 */
		bool				IsTransformUpdated() noexcept { return m_isTransformUpdated; }

		/**
		 * Destructor of the object. Destroys all of its children and components.
		 */
		~Object() noexcept
		{
			if (m_parent != nullptr)
				m_parent->RemoveChild(this);
		}

		/**
		 * Sets the object for destruction. Calls OnDestroy on every components
		 * and Destroy on all of its childs
		 */
		void				Destroy() noexcept;

		/**
		 * Returns if the object was marked for destruction
		 */
		bool				IsDestroyed() const noexcept
		{
			return m_isDestroyed;
		}

		/**
		 * Set the activity of the object
		 * @param active: New activity of the object
		 */
		void				SetActivity(bool active) noexcept
		{
			m_isActive = active;
			if (active)
			{
				for (auto it : m_childs)
					it->SetActivity(active);
			}
		}

		/**
		 * Get the activity of the object
		 */
		bool				IsActive() const noexcept
		{
			return m_isActive;
		}

		/**
		 * Creates a child with given transform
		 * @param localPos: Local position of the object
		 * @return Pointer to created object
		 */
		Object*				CreateChild(const std::string& name, const Transform& localPos) noexcept;

		/**
		 * Removes given object from children if it is one
		 * @param object: Pointer to object to remove
		 */
		void				RemoveChild(Object* object) noexcept;
		
		/**
		 * Adds given object to children
		 * @param object: Pointer to object to add
		 */
		void				AddChild(Object* object) noexcept;

		/**
		 * Returns a pointer to the parent of the object
		 * If the pointer is nullptr, the object is the root node
		 * @return Pointer to the parent
		 */
		inline Object*		GetParent() const noexcept;

		/**
		 * Sets parent to given object. Removes itself from previous parent if needed
		 */
		void				SetParent(Object* parent) noexcept;

		/**
		 * Get children of the object
		 */
		inline std::list<Object*>&	GetChildren() noexcept;

		/**
		 * Set an event call when the transform of the Object change
		 */
		inline Core::SystemManagement::ID SetAnEventTransformChange(std::function<void()> function)
		{
			return m_EventTransformChange += function;
		}

		inline void DeleteAnEventTransformChange(Core::SystemManagement::ID ID)
		{
			m_EventTransformChange.RemoveListener(ID);
		}

		/**
		 * Set an event call when the transform of the Object change
		 */
		inline void RemoveEventTransformChange()
		{
			m_EventTransformChange.RemoveAllListeners();
		}

		/**
		 * Translates the object in local space by given vector
		 * @param v: Vector to translate by
		 * @return Const reference to position of the object
		 */
		const Maths::Vec3&	Translate(const Maths::Vec3& v) noexcept
		{
			const Maths::Vec3& temp = m_transform.Translate(v);
			RequireUpdate();
			return temp;
		}
		/**
		 * Rotates the object in local space by given vector
		 * @param q: Quaternion to rotate by
		 * @return Const reference to rotation of the object
		 */
		const Maths::Quat&	Rotate(const Maths::Quat& q) noexcept
		{
			const Maths::Quat& temp = m_transform.Rotate(q);
			RequireUpdate();
			return temp;
		}

		/**
		 * Rotate object towards given direction
		 * @param target: New forward at the end of the rotation
		 * @param rotation: Slerp value to determine the rotation between current and target (must be between 0 and 1)
		 */
		void	RotateTowards(const Maths::Vec3& target, const float& rotation)
		{
			m_transform.RotateTowards(target, rotation);
			RequireUpdate();
		}

		/**
		 * Scales the object in local space by given vector
		 * @param v: Vector to scale by
		 * @return Const reference to scale of the object
		 */
		const Maths::Vec3&	Scale(const Maths::Vec3& v) noexcept
		{
			const Maths::Vec3& temp = m_transform.Scale(v);
			RequireUpdate();
			return temp;
		}

		/**
		 * Translates the object in global space by given vector
		 * @param v: Vector to translate by
		 * @return Const reference to position of the object
		 */
		void TranslateGlobal(const Maths::Vec3& v) noexcept
		{
			m_transform.TranslateGlobal(m_parent->GetUpdatedTransform(), v);
			RequireUpdate();
		}
		/**
		 * Rotates the object in global space by given vector
		 * @param q: Quaternion to rotate by
		 * @return Const reference to rotation of the object
		 */
		void RotateGlobal(const Maths::Quat& q) noexcept
		{
			m_transform.RotateGlobal(m_parent->GetUpdatedTransform(), q);
			RequireUpdate();
		}
		/**
		 * Scales the object in global space by given vector
		 * @param v: Vector to scale by
		 * @return Const reference to scale of the object
		 */
		void ScaleGlobal(const Maths::Vec3& v) noexcept
		{
			m_transform.ScaleGlobal(m_parent->GetUpdatedTransform(), v);
			RequireUpdate();
		}

		/**
		 * Set local position of object to given value
		 * @param pos: New local position
		 */
		void				SetPos(const Maths::Vec3& pos) noexcept
		{
			m_transform.SetLocalPos(pos);
			RequireUpdate();
		}
		/**
		 * Set local rotation of object to given value
		 * @param rot: New local rotation
		 */
		void				SetRot(const Maths::Quat& rot) noexcept
		{
			m_transform.SetLocalRot(rot);
			RequireUpdate();
		}
		/**
		 * Set local scale of object to given value
		 * @param scale: New local scale
		 */
		void				SetScale(const Maths::Vec3& scale) noexcept
		{
			m_transform.SetLocalScale(scale);
			RequireUpdate();
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
			{
				if (m_parent)
					m_transform.UpdatePos(m_parent->GetUpdatedTransform());
				else
					m_transform.UpdatePos();
			}
			return m_transform.GetGlobalPos();
		}
		/**
		 * Returns the global rotation of the object
		 * @return Local rotation of the object
		 */
		const Maths::Quat&	GetGlobalRot() noexcept
		{
			if (!m_transform.IsGPosUpdated())
			{
				if (m_parent)
					m_transform.UpdatePos(m_parent->GetUpdatedTransform());
				else
					m_transform.UpdatePos();
			}
			return m_transform.GetGlobalRot();
		}
		/**
		 * Returns the global scale of the object
		 * @return Local scale of the object
		 */
		const Maths::Vec3&	GetGlobalScale() noexcept
		{
			if (!m_transform.IsGPosUpdated())
			{
				if (m_parent)
					m_transform.UpdatePos(m_parent->GetUpdatedTransform());
				else
					m_transform.UpdatePos();
			}
			return m_transform.GetGlobalScale();
		}

		/**
		 * Sets the global position of the object
		 * @param pos: Global pos of the object
		 */
		void SetGlobalPos(const Maths::Vec3& pos) noexcept
		{
			m_transform.SetGlobalPos(m_parent->GetUpdatedTransform(), pos);
			RequireUpdate();
		}
		/**
		 * Sets the global rotation of the object
		 * @param rot: Global rotation of the object
		 */
		void SetGlobalRot(const Maths::Quat& rot) noexcept
		{
			m_transform.SetGlobalRot(m_parent->GetUpdatedTransform(), rot);
			RequireUpdate();
		}
		/**
		 * Sets the global scale of the object
		 * @param scale: Global scale of the object
		 */
		void SetGlobalScale(const Maths::Vec3& scale) noexcept
		{
			m_transform.SetGlobalScale(m_parent->GetUpdatedTransform(), scale);
			RequireUpdate();
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
			{
				if (m_parent != nullptr)
					m_transform.UpdatePos(m_parent->GetUpdatedTransform());
				else
					m_transform.UpdatePos();
			}
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

		std::list<ComponentBase*>& GetComponents() noexcept;

		const std::string& GetName() const noexcept
		{
			return m_name;
		}

		void SetName(const std::string& name) noexcept
		{
			m_name = name;
		}

		/**
		 * Returns root object of the scene
		 */
		RootObject* GetScene() const noexcept
		{
			return m_root;
		}

		/**
		 * Returns the forward vector of the object
		 */
		Core::Maths::Vec3	Forward() noexcept
		{
			constexpr Core::Maths::Vec3 forward{ 0, 0, 1 };
			constexpr Core::Maths::Quat	forQuat{ 0, forward };
			return (GetGlobalRot() * forQuat * GetGlobalRot().Inversed()).GetVec();
		}

		/**
		 * Returns the up vector of the object
		 */
		Core::Maths::Vec3	Up() noexcept
		{
			constexpr Core::Maths::Vec3 up{ 0, 1, 0 };
			constexpr Core::Maths::Quat	upQuat{ 0, up };
			return (GetGlobalRot() * upQuat * GetGlobalRot().Inversed()).GetVec();
		}

		/**
		 * Returns the right vector of the object
		 */
		Core::Maths::Vec3	Right() noexcept
		{
			constexpr Core::Maths::Vec3 right{ 1, 0, 0 };
			constexpr Core::Maths::Quat	rightQuat{ 0, right };
			return (GetGlobalRot() * rightQuat * GetGlobalRot().Inversed()).GetVec();
		}

		/**
		 * Checks if given object is a child of this object.
		 * Also checks if it is a child of its child etc
		 * @param o: Object to check
		 * @return Whether it is a child or not
		 */
		bool				HasChild(Object* o) const noexcept;

		/**
		 * Returns all components of the type Component contained
		 * in this object and its childs. 
		 */
		template <class Component>
		void	GetComponentsOfType(std::list<Component*>& components) const noexcept
		{
			GetComponentsOfTypeInObject<Component>(components);
			GetComponentsOfTypeInChilds<Component>(components);
		}

		/**
		 * Returns all components of the type Component contained
		 * in the childs of this object
		 */
		template <class Component>
		void	GetComponentsOfTypeInChilds(std::list<Component*>& components) const noexcept
		{
			for (auto it : m_childs)
			{
				it->GetComponentsOfType(components);
			}
		}

		/**
		 * Returns all components of the type Component contained
		 * in this object. Will not get them from children
		 */
		template <class Component>
		void	GetComponentsOfTypeInObject(std::list<Component*>& components) const noexcept
		{
			for (auto it : m_components)
			{
				if (rttr::type::get<Component>() == it->get_type())
					components.push_back(dynamic_cast<Component*>(it));
			}
		}

		/**
		 * Returns all components of the base type Component contained
		 * in this object and its childs.
		 */
		template <class Component>
		void	GetComponentsOfBaseType(std::list<Component*>& components) const noexcept
		{
			GetComponentsOfBaseTypeInObject<Component>(components);
			GetComponentsOfBaseTypeInChilds<Component>(components);
		}

		/**
		 * Returns all components of the base type Component contained
		 * in the childs of this object
		 */
		template <class Component>
		void	GetComponentsOfBaseTypeInChilds(std::list<Component*>& components) const noexcept
		{
			for (auto it : m_childs)
			{
				it->GetComponentsOfBaseType(components);
			}
		}

		/**
		 * Returns all components of the base type Component contained
		 * in this object. Will not get them from children
		 */
		template <class Component>
		void	GetComponentsOfBaseTypeInObject(std::list<Component*>& components) const noexcept
		{
			for (auto it : m_components)
			{
				if (rttr::type::get<Component>().is_base_of(it->get_type()))
					components.push_back(dynamic_cast<Component*>(it));
			}
		}

		REGISTER_CLASS()
	};

	inline Object*	Datastructure::Object::GetParent() const noexcept
	{
		return m_parent;
	}

	inline std::list<Object*>& Datastructure::Object::GetChildren() noexcept
	{
		return m_childs;
	}

	inline std::list<ComponentBase*>& Datastructure::Object::GetComponents() noexcept
	{
		return m_components;
	}
}
