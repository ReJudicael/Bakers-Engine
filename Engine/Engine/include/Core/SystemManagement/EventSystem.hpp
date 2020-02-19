#pragma once

#include <unordered_map>
#include <functional>

namespace SystemManagement
{
    /**
     * Handle callback hierarchy and event system
     * @tparam Args: Params of method sent
     * @param ID: Listener's ID to identify its callback
     * @param Event: Function associated to Event
     */
    using ID = unsigned int;
    template<class... Args>
    class EventSystem final
    {
    private:
        using Event = std::function<void(Args...)>;

        std::unordered_map<ID, Event> m_eventIDs;
        int m_availableID{ 0 };

    public:
        /**
         * Add a listener to the event
         * @param call: Callback function
         * @return The ID of created listener
         */
        ID AddListener(const Event& call) noexcept;

        /**
         * Remove a listener through its ID
         * @param id: ID of listener to remove
         * @return Whether the deletion work or not
         */
        bool RemoveListener(ID id) noexcept;

        /**
         * Remove all listeners
         */
        void RemoveAllListeners() noexcept;

        /**
         * Call each stored functions attached to its event
         * @params args: Arguments for the call (Variadic arguments)
         */
        void Invoke(Args... args) noexcept;

        /**
         * Get the number of listeners
         * @return Number of listeners
         */
        int GetEventCount() const noexcept;
    };

    template<class... Args>
    inline ID EventSystem<Args...>::AddListener(const Event& call) noexcept
    {
        // Increment m_availableID after use
        ID id = m_availableID++;
        m_eventIDs.emplace(id, call);
        return id;
    }

    template<class... Args>
    inline bool EventSystem<Args...>::RemoveListener(ID listenerID) noexcept
    {
        return m_eventIDs.erase(listenerID) != 0;
    }

    template<class... Args>
    inline void EventSystem<Args...>::RemoveAllListeners() noexcept
    {
        m_eventIDs.clear();
    }

    template<class... Args>
    inline void EventSystem<Args...>::Invoke(Args... args) noexcept
    {
        for (auto [Key, T] : m_eventIDs)
            T(args...);
    }

    template<class ...Args>
    inline int EventSystem<Args...>::GetEventCount() const noexcept
    {
        return m_eventIDs.size();
    }
}
