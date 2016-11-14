#include "stdafx.h"
#include "Common.h"
#include "JobManager.h"
#include <thread>
#include <mutex>

JobManager::JobManager() : m_tasksInQueueCounter(0), m_queueCapacity(0)
{
}

void JobManager::Init()
{
	m_queueCapacity = INITIAL_QUEUE_CAPACITY2;
	UInt32 nbConsumerThreads = global->settings->ReadConfigValue_uint("system.JobManager_nb_threads");	// #TODO The number of threads should not be too high -> use std::thread::hardware_concurrency() and substact the correct ammount

	for (UInt32 i = 0; i < nbConsumerThreads; i++)
	{
		std::thread thread([this] { this->Consume(); });
		thread.detach();
		m_workerThreads.push_back(std::move(thread));
	}
}

JobManager::~JobManager()
{
	m_tasksInQueueCounter = 0;
}

void JobManager::Push(LambdaFunction callback)
{
	while (!m_lockFreeQueue.push(new LambdaFunction(callback)));
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

void JobManager::Consume()
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
			LambdaFunction* function;

			if (m_lockFreeQueue.pop(function))
			{
				--m_tasksInQueueCounter;

				(*function)();	// Call the lambda function

				delete function;
			}
		}
	}
}

void JobManager::ResizeQueue(size_t requestedQueueCapacity)
{
	ASSERT_MSG(requestedQueueCapacity >= INITIAL_QUEUE_CAPACITY2, "JobManager: Requested queue capacity should not be lower than the default value of DEFAULT_QUEUE_CAPACITY");

	m_lockFreeQueue.reserve(requestedQueueCapacity);

	m_queueCapacity = m_queueCapacity + (Int64)requestedQueueCapacity;
}

Int64 JobManager::GetTasksInQueueCount()
{
	return m_tasksInQueueCounter;
}