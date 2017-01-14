#pragma once
#include <mutex>
#include <boost/thread/shared_mutex.hpp>

using read_mutex = boost::shared_lock<boost::shared_mutex>;
using exclusive_mutex = std::lock_guard<boost::shared_mutex>;

template <typename T>
class ReadWriteMutexWrapper
{
	mutable boost::shared_mutex m_sharedLock;
	mutable T m_data;

public:
	ReadWriteMutexWrapper()
	{
	}

	ReadWriteMutexWrapper(const T& data) : m_data(data)
	{
	}

	T& Get() const
	{
		return m_data;
	}

	T* operator->() const
	{
		return &m_data;
	}

	T* operator->()
	{
		return &m_data;
	}

	void Set(const T& input)
	{
		m_data = input;
	}

	boost::shared_mutex& GetMutex() const
	{
		return m_sharedLock;
	}

	ReadWriteMutexWrapper& operator=(const T& input)
	{
		Set(input);

		return *this;
	}

	ReadWriteMutexWrapper& operator=(const T&& input)
	{
		Set(input);

		return *this;
	}
};