#pragma once
#include <functional>

struct IJobManager
{
	typedef std::function<void()> LambdaFunction;
	
	virtual void Init() = 0;
	virtual void Push(LambdaFunction callback) = 0;
	virtual long int GetTasksInQueueCount() = 0;
};