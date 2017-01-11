#ifndef PROCESSING_QUEUE_HEADER
#define PROCESSING_QUEUE_HEADER

#include <memory>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>
#include <deque>
#include <atomic>

#include "Task.h"

using namespace std;

class CountdownEvent
{
private:
	mutex m_mutex;
	condition_variable m_condition;
	int m_currentCount;

public:
	explicit CountdownEvent (int initialCount) :
		m_currentCount (initialCount)
	{
	}

	bool Signal()
	{
		{
			std::unique_lock<std::mutex> lock(this->m_mutex);
			if (this->m_currentCount > 0)
			{
				this->m_currentCount--;
				if (this->m_currentCount == 0)
				{
					this->m_condition.notify_all();
					return true;
				}
			}
		}

		return false;
	}

	void Wait()
	{
		std::unique_lock<std::mutex> lock(this->m_mutex);
		while (this->m_currentCount > 0)
		{
			this->m_condition.wait(lock);
		}
	}
};

// END OF TASK

// BLOCKING QUEUE
template<class T> class BlockingQueue
{
private:
	mutex m_mutex;
	condition_variable m_condition;
	deque<T> m_queue;
public:
	void Enqueue(T item)
	{
		{
			std::unique_lock<std::mutex> lock(this->m_mutex);
			this->m_queue.push_front(item);
		}

		this->m_condition.notify_one();
	}

	bool TryTake(T &item, int durationInMs)
	{
		chrono::milliseconds timeout(durationInMs);
		std::unique_lock<std::mutex> lock(this->m_mutex);
		if (!this->m_condition.wait_for(lock, timeout, [=]{ return !this->m_queue.empty(); })) 
		{
			return false;
		}

		item = std::move (this->m_queue.back());
		this->m_queue.pop_back();
		return true;
	}
};
// END OF QUEUE

// PROCESSING QUEUE
class ProcessingQueue 
{
private:
	int concerrencyLevel;
	int timeToWaitInMs;
	atomic<int> status;
	CountdownEvent countdown;

	mutex m_mutex;

	vector<thread> threads;
	BlockingQueue<shared_ptr<Task>> workItems;

	bool SetRunning();
	bool SetStopping();
	bool SetStopped();

	void WorkerThread();

public:
	explicit ProcessingQueue(int concerrencyLevel, int timeToWaitInMs);
	bool Add(shared_ptr<Task> task);
	void Start();
	void Stop();
	void WaitForCompletion();
};
// END OF PROCESSING QUEUE
#endif