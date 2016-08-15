#pragma once
#include <boost\lockfree\queue.hpp>
#include <boost\atomic.hpp>
#include <boost\thread.hpp>

#define DEFAULT_QUEUE_CAPACITY 16

class ProducerConsumerQueue
{
private:
	typedef std::function<void()> lambdaFunction;

	boost::lockfree::queue<lambdaFunction*> m_queue{ DEFAULT_QUEUE_CAPACITY };
	boost::thread_group m_threads;
	boost::atomic<long int> m_tasksInQueueCount;
	boost::atomic<long int> m_queueCapacity;
	boost::condition_variable m_conditionVariable;
	boost::mutex m_mutex;
	const std::string m_name;

public:
	ProducerConsumerQueue(size_t, const char*, size_t queueInitialCapacity = DEFAULT_QUEUE_CAPACITY);
	~ProducerConsumerQueue();

	void Push(lambdaFunction callback);

private:
	void Consume();
	void ResizeQueue(size_t);
};