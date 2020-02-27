#pragma once

#include <queue>
#include <future>
#include <chrono>
#include "Debug.h"

namespace Core::SystemManagement
{
	/**
	 * System to handle multithreading
	 */
	class TaskSystem
	{
	private:
		using Task = std::function<void()>;
		std::queue<Task>			m_queueTask;
		std::vector<std::thread>	m_threads;
		TracyLockableN(std::mutex, m_lock, "TaskSystem mutex")
		std::condition_variable_any	m_conditionVariable;
		std::atomic<bool>			m_isAllowed;	// Accept function or not

		unsigned int				m_maxThreads;

	public:
		/**
		 * Give task-receiving method to threads
		 */
		TaskSystem() noexcept;

		/**
		 * Stop all threads and clear data
		 */
		~TaskSystem();

	private:
		/**
		 * Method allowing each thread to do its task
		 * @param indexThread: Index of the thread calling this method
		 */
		void WaitTaskLoop(int i) noexcept;

	public:
		/**
		 * Bind a callable object that will be bound to some arguments
		 * @tparam f: Callable object
		 * @tparam args: List of arguments to bind
		 */
		template<class Function, class... Args>
		void AddTask(Function&& f, Args&& ... args) noexcept;

		/**
		 * Bind an object to be forwarded
		 * @tparam f: The object to be forwarded
		 */
		template<class Function>
		void AddTask(Function&& f) noexcept;

		/**
		 * Bind a callable object with an implicit return type and deduces the return type
		 * @tparam f: Callable object
		 * @tparam args: List of arguments to bind
		 * @return return a std::future of result of callable object
		 */
		template <class Function, class... Args>
		std::future<std::invoke_result_t<Function(Args...)>> AddTask_t(Function&& f, Args&& ... args) noexcept;

		/**
		 * End Task System by joining each thread
		 */
		void EndTaskSystem() noexcept;

		/**
		 * Get the number of concurrent threads supported by the implementation
		 * @return The number of concurrent threads supported by the implementation
		 */
		unsigned int GetMaxThreads() const noexcept;
	};

	inline TaskSystem::TaskSystem() noexcept :
		m_maxThreads{ std::thread::hardware_concurrency() }
	{
		m_isAllowed.store(true);

		// Give task-receiving method to threads
		for (unsigned int i = 0; i < m_maxThreads; ++i)
			m_threads.push_back(std::thread(&TaskSystem::WaitTaskLoop, this, i));
	}

	inline TaskSystem::~TaskSystem()
	{
		// If EndTaskSystem has not been used, call EndTaskSystem in destructor
		if (m_isAllowed.load())
			EndTaskSystem();

		// Clear threads vector
		m_threads.clear();
	}

	inline void TaskSystem::WaitTaskLoop(int i) noexcept
	{
		SET_THREAD_NAME((std::string("Worker number ") + std::to_string(i)).c_str())
		Task task;
		while (true)
		{
			{
				std::unique_lock<LockableBase(std::mutex)> queueTaskLock(m_lock);
				LockMark(m_lock)
				m_conditionVariable.wait(queueTaskLock, [this] { return !m_isAllowed.load() || !m_queueTask.empty(); });

				if (!m_isAllowed.load() && m_queueTask.empty())
					return;

				task = m_queueTask.front(); // Take first task in queueTask
				m_queueTask.pop();
			}
			{
				ZoneScopedN("Executing task")
					ZoneText("Thread is executing a task", 27)
				task(); // Thread does its task
			}
		}
	}

	template<class Function, class ...Args>
	inline void TaskSystem::AddTask(Function&& f, Args&& ...args) noexcept
	{
		{
			std::unique_lock<LockableBase(std::mutex)> lock(m_lock);
			LockMark(m_lock)

			// Add function in queueTask
			m_queueTask.emplace(std::_Binder<std::_Unforced, Function, Args...>
				(static_cast<Function&&>(f), static_cast<Args&&>(args)...));
		}
		m_conditionVariable.notify_one();
	}

	template<class Function>
	inline void TaskSystem::AddTask(Function&& f) noexcept
	{
		{
			std::unique_lock<LockableBase(std::mutex)> lock(m_lock);
			LockMark(m_lock)

			// Add function in queueTask
			m_queueTask.emplace(std::forward<Function>(f));
		}
		m_conditionVariable.notify_one();
	}

	template<class Function, class ...Args>
	inline std::future<std::invoke_result_t<Function(Args...)>> TaskSystem::AddTask_t(Function&& f, Args&& ...args) noexcept
	{
		using return_type = std::result_of_t<Function(Args...)>;

		std::shared_ptr<std::packaged_task<return_type()>> task{ std::make_shared<std::packaged_task<return_type()>>
			(std::bind(std::forward<Function>(f), std::forward<Args>(args)...)) };

		std::future<return_type> res{ task->get_future() };

		{
			std::unique_lock<LockableBase(std::mutex)> lock(m_lock);
			LockMark(m_lock)
			// Add function in queueTask
			m_queueTask.emplace([task] { (*task)(); });
		}
		m_conditionVariable.notify_one();

		return res;
	}

	inline void TaskSystem::EndTaskSystem() noexcept
	{
		ZoneScoped
			ZoneText("Closing task system", 20)
		{
			std::unique_lock<LockableBase(std::mutex)> lock(m_lock);
			LockMark(m_lock)
			m_isAllowed.store(false);
		}
		m_conditionVariable.notify_all();

		for (unsigned int i = 0; i < m_maxThreads; ++i)
			m_threads.at(i).join(); // Join each thread
	}

	inline unsigned int TaskSystem::GetMaxThreads() const noexcept
	{
		return m_maxThreads;
	}
}
