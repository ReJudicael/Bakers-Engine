#pragma once

#include <unordered_map>
#include <functional>
#include "Debug.h"

#define BIND_EVENT(x) std::bind(&x, this, std::placeholders::_1)
#define BIND_EVENT_2(x) std::bind(&x, this, std::placeholders::_1, std::placeholders::_2)

namespace Core
{
	/**
	 * Contains all system tools of the engine
	 */
	namespace SystemManagement
	{
		typedef unsigned int ID;

		/**
		 * Handle callback hierarchy and event system
		 * @tparam Args: Params of method sent
		 */
		template<class... Args>
		class EventSystem final
		{
		private:
			using Event = std::function<void(Args...)>;

			/**
			 * Each Listener has its Event
			 * @T1 ID: Listener's ID to identify its callback
			 * @T2 Event: Function associated to Event
			 */
			std::unordered_map<ID, Event> m_listenerEventMap;

			/**
			 * Available ID to create a new Listener
			 */
			ID m_availableIDForNewListener{ 1 };

		public:
			/**
			 * Default constructor
			 */
			EventSystem() = default;

			/**
			 * Default destructor
			 */
			~EventSystem() = default;

			/**
			 * Call each stored functions attached to its event
			 * @param args: Arguments for the call (Variadic arguments)
			 */
			void operator()(Args... args) noexcept;

			/**
			 * Add a listener to the event
			 * @param call: Callback function
			 * @return The ID of created listener
			 */
			ID operator+=(const Event& call) noexcept;

			/**
			 * Remove a listener through its ID
			 * @param listenerID: ID of listener to remove
			 * @return Whether the deletion work or not
			 */
			bool operator-=(ID listenerID) noexcept;

			/**
			 * Add a listener to the event
			 * @param call: Callback function
			 * @return The ID of created listener
			 */
			ID AddListener(const Event& call) noexcept;

			/**
			 * Remove a listener through its ID
			 * @param listenerID: ID of listener to remove
			 * @return Whether the deletion work or not
			 */
			bool RemoveListener(ID listenerID) noexcept;

			/**
			 * Remove all listeners
			 */
			void RemoveAllListeners() noexcept;

			/**
			 * Call each stored functions attached to its event
			 * @param args: Arguments for the call (Variadic arguments)
			 */
			void Invoke(Args... args) noexcept;

			/**
			 * Get the number of event registered
			 * @return Number of event registered
			 */
			size_t GetEventCount() const noexcept;

			/**
			 * Get ID of last listener
			 * @return ID of last listener or 0 if there is no listener
			 */
			ID GetLastListenerID() const noexcept;
		};

		template<class... Args>
		inline void EventSystem<Args...>::operator()(Args... args) noexcept
		{
			Invoke(args...);
		}

		template<class... Args>
		inline ID EventSystem<Args...>::operator+=(const Event& call) noexcept
		{
			return AddListener(call);
		}

		template<class... Args>
		inline bool EventSystem<Args...>::operator-=(ID listenerID) noexcept
		{
			return RemoveListener(listenerID);
		}

		template<class... Args>
		inline ID EventSystem<Args...>::AddListener(const Event& call) noexcept
		{
			m_listenerEventMap.emplace(m_availableIDForNewListener, call);
			return m_availableIDForNewListener++;
		}

		template<class... Args>
		inline bool EventSystem<Args...>::RemoveListener(ID listenerID) noexcept
		{
			return m_listenerEventMap.erase(listenerID) != 0;
		}

		template<class... Args>
		inline void EventSystem<Args...>::RemoveAllListeners() noexcept
		{
			m_listenerEventMap.clear();
		}

		template<class... Args>
		inline void EventSystem<Args...>::Invoke(Args... args) noexcept
		{
			for (const auto& [Key, T] : m_listenerEventMap)
				T(args...);
		}

		template<class ...Args>
		inline size_t EventSystem<Args...>::GetEventCount() const noexcept
		{
			return m_listenerEventMap.size();
		}

		template<class ...Args>
		inline ID EventSystem<Args...>::GetLastListenerID() const noexcept
		{
			return m_availableIDForNewListener - 1;
		}
	}
}
