#pragma once
#include "IJobManager.h"
#include <boost\lockfree\queue.hpp>
#include <boost\atomic.hpp>
#include <boost\thread.hpp>

#define INITIAL_QUEUE_CAPACITY 1024

class JobManager : public IJobManager
{
public:
	JobManager();
	~JobManager();
	void Init();

	void Push(LambdaFunction callback);
	long int GetTasksInQueueCount();

private:
	boost::lockfree::queue<LambdaFunction*> m_lockFreeQueue{ INITIAL_QUEUE_CAPACITY };
	boost::thread_group m_workerThreads;
	boost::atomic<long int> m_tasksInQueueCount;
	boost::atomic<long int> m_queueCapacity;
	boost::condition_variable m_conditionVariable;
	boost::mutex m_mutex;
	const std::string m_name;

private:
	void Consume();
	void ResizeQueue(size_t);
};