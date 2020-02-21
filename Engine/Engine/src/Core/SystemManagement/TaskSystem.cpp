#include "TaskSystem.hpp"

namespace Core::SystemManagement
{
	TaskSystem::TaskSystem() : m_maxThreads{ std::thread::hardware_concurrency() }
	{
		m_isAllowed.store(true);

		// Give task-receiving method to threads
		for (unsigned int i = 0; i < m_maxThreads; ++i)
			m_threads.push_back(std::thread(&TaskSystem::WaitTaskLoop, this));
	}

	TaskSystem::~TaskSystem()
	{
		// If EndTaskSystem has not been used, call EndTaskSystem in destructor
		if (m_isAllowed.load())
			EndTaskSystem();

		// Clear threads vector
		m_threads.clear();
	}

	void TaskSystem::WaitTaskLoop() noexcept
	{
		Task task;
		while (true)
		{
			{
				std::unique_lock<std::mutex> queueTaskLock(m_lock);
				m_conditionVariable.wait(queueTaskLock, [this] { return !m_isAllowed.load() || !m_queueTask.empty(); });

				if (!m_isAllowed.load() && m_queueTask.empty())
					return;

				task = m_queueTask.front(); // Take first task in queueTask
				m_queueTask.pop();
			}
			task(); // Thread does its task
		}
	}

	void TaskSystem::EndTaskSystem() noexcept
	{
		{
			std::unique_lock<std::mutex> lock(m_lock);
			m_isAllowed.store(false);
		}
		m_conditionVariable.notify_all();

		for (unsigned int i = 0; i < m_maxThreads; ++i)
			m_threads.at(i).join(); // Join each thread
	}
}
