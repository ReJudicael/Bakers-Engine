#pragma once
namespace Core::Datastructure
{
	class Object;

	class ComponentBase
	{
	private:
		bool		m_isDestroyed = false;
		bool		m_isInit = false;
	protected:
		bool		m_isActive = false;
		Object*		m_parent{ nullptr };
	public:
		/**
		 * Called on start of the component, on the first frame it
		 * is active
		 */
		virtual void	OnStart() = 0;
		/**
		 * Called when destruction is asked. Destructor will be called
		 * at the end of the frame
		 */
		virtual void	OnDestroy() = 0;

		ComponentBase(const ComponentBase& c) = default;
		ComponentBase() = default;
		~ComponentBase() = default;

		void			SetParent(Object* parent)
		{
			m_parent = parent;
		}

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
	};
}
