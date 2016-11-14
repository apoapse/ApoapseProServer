#include "stdafx.h"
#include "ThreadPool.h"
#include "Common.h"

ThreadPool::ThreadPool(const string& threadPoolName, UInt32 nbThreads)
	: threadPoolName(threadPoolName),
	m_queueCapacity(INITIAL_QUEUE_CAPACITY),
	m_tasksInQueueCounter(0)
{
	for (UInt32 i = 0; i < nbThreads; i++)
	{
		std::thread thread([this] { this->Consume(); });
		thread.detach();

		m_workerThreads.push_back(std::move(thread));
	}

	//Log -> created x threads
	Log("Created worker threads");
}

ThreadPool::~ThreadPool()
{
	m_tasksInQueueCounter = 0;
	m_conditionVariable.notify_all();
}

void ThreadPool::OnAddedTask()
{
	++m_tasksInQueueCounter;

	ASSERT(m_tasksInQueueCounter > 0);

	m_conditionVariable.notify_one();

	if (m_tasksInQueueCounter >= (Int64)(m_queueCapacity - 2))	// Margin of 2 items
	{
		Int64 newSize = m_queueCapacity * 2;

		Log("JobManager: queue close to be full, preventively resizing to " + std::to_string(newSize));
		ResizeQueue(newSize);
	}
}

void ThreadPool::Consume()
{
	while (true)
	{
		if (m_tasksInQueueCounter == 0)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_conditionVariable.wait(lock);
		}
		else
		{
			ITask* task;
			if (m_workQueue.pop(task))
			{
				--m_tasksInQueueCounter;

				task->Execute();
				delete task;
			}
		}
	}
}

void ThreadPool::ResizeQueue(size_t requestedQueueCapacity)
{
	ASSERT_MSG(requestedQueueCapacity >= INITIAL_QUEUE_CAPACITY, "JobManager: Requested queue capacity should not be lower than the default value of DEFAULT_QUEUE_CAPACITY");

	m_workQueue.reserve(requestedQueueCapacity);

	m_queueCapacity = m_queueCapacity + (Int64)requestedQueueCapacity;
}

Int64 ThreadPool::GetTasksInQueueCount()
{
	return m_tasksInQueueCounter;
}