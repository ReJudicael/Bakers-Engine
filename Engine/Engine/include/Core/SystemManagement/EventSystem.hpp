#pragma once

#include <unordered_map>
#include <functional>



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
        ID m_availableListenerID{ 0 };

    public:
        /**
         * Add a listener to the event
         * @param call: Callback function
         * @return The ID of created listener
         */
        ID operator+=(const Event& call) noexcept;

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
        bool RemoveListener(ID listenerID) noexcept;

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
         * Get the number of event registered
         * @return Number of event registered
         */
        size_t GetEventCount() const noexcept;
    };

    template<class... Args>
    inline ID EventSystem<Args...>::operator+=(const Event& call) noexcept
    {
        return AddListener(call);
    }

    template<class... Args>
    inline ID EventSystem<Args...>::AddListener(const Event& call) noexcept
    {
        ID listenerID = m_availableListenerID;
        m_listenerEventMap.emplace(listenerID, call);
        m_availableListenerID++;
        return listenerID;
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
}
