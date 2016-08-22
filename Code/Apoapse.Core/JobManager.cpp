#include "stdafx.h"
#include "Common.h"
#include "JobManager.h"

JobManager::JobManager() : m_tasksInQueueCount(0), m_queueCapacity(0)
{
	global->settings->RegisterConfigVar_uint("system.JobManager_nb_threads", 16);
}

void JobManager::Init()
{
	m_queueCapacity = INITIAL_QUEUE_CAPACITY;
	int nbConsumerThreads = global->settings->ReadConfigValue_uint("system.JobManager_nb_threads");

	for (size_t i = 0; i < nbConsumerThreads; i++)
	{
		m_workerThreads.create_thread([this] { this->Consume(); });
	}

	Log("JobManager initialized");
}

JobManager::~JobManager()
{
	//TODO: What's happen when the queue is deleted but there are still things in the queue?
}

void JobManager::Push(LambdaFunction callback)
{
	while (!m_lockFreeQueue.push(new LambdaFunction(callback)));
	++m_tasksInQueueCount;

	ASSERT(m_tasksInQueueCount > 0);

	m_conditionVariable.notify_all();


	if (m_tasksInQueueCount >= (m_queueCapacity - 2))	// Margin of 2 items
	{
		long newSize = m_queueCapacity * 2;

		Log("JobManager: queue close to be full, preventively resizing to " + std::to_string(newSize));
		ResizeQueue(newSize);
	}
}

void JobManager::Consume()
{
	while (true)
	{
		boost::unique_lock<boost::mutex> lock(m_mutex);

		while (m_tasksInQueueCount == 0)
		{
			m_conditionVariable.wait(lock);
		}

		LambdaFunction* function;

		if (m_lockFreeQueue.pop(function))
		{
			--m_tasksInQueueCount;

			(*function)();	//	Call the lambda function

			delete function;
		}
	}
}

void JobManager::ResizeQueue(size_t requestedQueueCapacity)
{
	ASSERT_MSG(requestedQueueCapacity >= INITIAL_QUEUE_CAPACITY, "JobManager: Requested queue capacity should not be lower than the default value of DEFAULT_QUEUE_CAPACITY");

	m_lockFreeQueue.reserve(requestedQueueCapacity);

	m_queueCapacity = m_queueCapacity + (long int)requestedQueueCapacity;
}

long int JobManager::GetTasksInQueueCount()
{
	return m_tasksInQueueCount;
}