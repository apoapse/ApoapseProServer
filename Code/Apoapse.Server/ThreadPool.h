#pragma once
#include <atomic>
#include <thread>
#include <condition_variable>
#include <future>
#include <boost\lockfree\queue.hpp>

#define INITIAL_QUEUE_CAPACITY 512

class ThreadPool
{
	struct ITask
	{
		virtual ~ITask() {};
		virtual void Execute() = 0;
	};

	template <typename T>
	struct Task : public ITask
	{
		std::packaged_task<T()> task;

		Task(std::function<T()> func) : task(func)
		{
		}

		void Execute()
		{
			task();
		}
	};

	std::vector<std::thread> m_workerThreads;
	std::atomic<Int64> m_tasksInQueueCounter;
	std::atomic<UInt64> m_queueCapacity;
	boost::lockfree::queue<ITask*> m_workQueue{ INITIAL_QUEUE_CAPACITY };	// We are using raw pointer here because the Boost lockfree queue require a trivial assignment operator
	std::condition_variable m_conditionVariable;
	std::mutex m_mutex;

public:
	const string threadPoolName;

	ThreadPool(const string& threadPoolName, UInt32 nbThreads);
	virtual ~ThreadPool();

	template<typename T>
	std::future<typename std::result_of<T()>::type> PushTask(const T func)
	{
		typedef typename std::result_of<T()>::type TypeResult;

		Task<TypeResult>* task = new Task<TypeResult>(std::move(func));
		std::future<TypeResult> response = task->task.get_future();

		while(!m_workQueue.push(std::move(task)));
		OnAddedTask();

		return response;
	}

	Int64 GetTasksInQueueCount();

protected:
	void Consume();

private:
	void OnAddedTask();
	void ResizeQueue(size_t);
};