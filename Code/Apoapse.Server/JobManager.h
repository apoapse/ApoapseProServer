#pragma once
#include "IJobManager.h"
#include <boost\lockfree\queue.hpp>
#include <condition_variable>
#include <atomic>

#define INITIAL_QUEUE_CAPACITY2 1024

class JobManager : public IJobManager
{
private:
	boost::lockfree::queue<LambdaFunction*> m_lockFreeQueue{ INITIAL_QUEUE_CAPACITY2 };
	//boost::thread_group m_workerThreads;
	std::vector<std::thread> m_workerThreads;
	std::atomic<Int64> m_tasksInQueueCounter;
	std::atomic<UInt64> m_queueCapacity;
	std::condition_variable m_conditionVariable;
	std::mutex m_mutex;
	const std::string m_name;	// #TODO

public:
	JobManager();
	virtual ~JobManager();
	void Init();

	void Push(LambdaFunction callback);
	Int64 GetTasksInQueueCount();



private:
	void Consume();
	void ResizeQueue(size_t);
};