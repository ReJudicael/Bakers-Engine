#pragma once

#include <queue>
#include <future>
#include <chrono>

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
		std::mutex					m_lock;
		std::condition_variable		m_conditionVariable;
		std::atomic<bool>			m_isAllowed;	// Accept function or not

		unsigned int m_maxThreads;

		/**
		 * Give task-receiving method to threads
		 */
		TaskSystem();

		/**
		 * Stop all threads and clear data
		 */
		~TaskSystem();

		/**
		 * Method allowing each thread to do its task
		 * @param indexThread: Index of the thread calling this method
		 */
		void WaitTaskLoop() noexcept;

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

	template<class Function, class ...Args>
	inline void TaskSystem::AddTask(Function&& f, Args&& ...args) noexcept
	{
		{
			std::unique_lock<std::mutex> lock(m_lock);

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
			std::unique_lock<std::mutex> lock(m_lock);

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
			std::unique_lock<std::mutex> lock(m_lock);
			// Add function in queueTask
			m_queueTask.emplace([task] { (*task)(); });
		}
		m_conditionVariable.notify_one();

		return res;
	}

	inline unsigned int TaskSystem::GetMaxThreads() const noexcept
	{
		return m_maxThreads;
	}
}
