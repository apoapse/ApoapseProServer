#include "stdafx.h"
#include "ProducerConsumerQueue.h"

ProducerConsumerQueue::ProducerConsumerQueue(size_t nbConsumerThreads, const char* name, size_t queueInitialCapacity) : m_tasksInQueueCount(0), m_name(name), m_queueCapacity(0)
{
	if (queueInitialCapacity > DEFAULT_QUEUE_CAPACITY)
		ResizeQueue(queueInitialCapacity);
	else
		m_queueCapacity = DEFAULT_QUEUE_CAPACITY;


	for (size_t i = 0; i < nbConsumerThreads; i++)
	{
		m_threads.create_thread([this] { this->Consume(); });
	}
}

ProducerConsumerQueue::~ProducerConsumerQueue()
{
	//TODO: What's happen when the queue is deleted but there are still things in the queue?
}

void ProducerConsumerQueue::Push(lambdaFunction callback)
{
	while (!m_queue.push(new lambdaFunction(callback)));
	++m_tasksInQueueCount;

	ASSERT_MSG(m_tasksInQueueCount > 0, "ProducerConsumerQueue can't have a negative task count");

	m_conditionVariable.notify_all();


	if (m_tasksInQueueCount >= (m_queueCapacity - 2))	// Margin of 2 items
	{
		auto newSize = m_queueCapacity * 2;

		//LOG("ProducerConsumerQueue (" + m_name + "): queue close to be full, preventively resizing to " + std::to_string(newSize));
		ResizeQueue(newSize);
	}
}

void ProducerConsumerQueue::Consume()
{
	while (true)
	{
		boost::unique_lock<boost::mutex> lock(m_mutex);

		while (m_tasksInQueueCount == 0)
		{
			m_conditionVariable.wait(lock);
		}

		lambdaFunction* function;

		if (m_queue.pop(function))
		{
			--m_tasksInQueueCount;

			(*function)();	//	Call the lambda function

			delete function;
		}
	}
}

void ProducerConsumerQueue::ResizeQueue(size_t requestedQueueCapacity)
{
	ASSERT_MSG(requestedQueueCapacity >= DEFAULT_QUEUE_CAPACITY, "Requested queue capacity should not be lower than the default value DEFAULT_QUEUE_CAPACITY");

	m_queue.reserve(requestedQueueCapacity);

	m_queueCapacity = m_queueCapacity + (long int)requestedQueueCapacity;
}