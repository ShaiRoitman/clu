#include "BlockingTaskQueue.h"

#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <deque>
#include <mutex>
#include <atomic>

using namespace std;

static int Created = 0;
static int Running = 1;
static int Stopping = 2;
static int Stopped = 3;

BlockingTaskQueue::BlockingTaskQueue(int queueSize)
{
	this->queueSize = queueSize;
	this->state.exchange(Created);
}

void BlockingTaskQueue::Start(int numberOfThreads)
{
	this->state.exchange(Running);
	for (int i = 0; i < numberOfThreads ; ++i)
	{
		auto newThread = new thread([=]() { this->WorkerThread(); });
		this->threads.push_back(newThread);
	}
}

void BlockingTaskQueue::WorkerThread()
{
	while (this->state.load() == Running || this->state.load() == Stopping)
	{
		Task* task;
		{
			std::unique_lock<std::mutex> flk(this->tasksMutex);
			this->cv.wait(flk, [=]() { return this->tasks.size() > 0 || this->state.load() == Stopping; });
			if (this->tasks.size() > 0)
			{
				task = *this->tasks.begin();
				this->tasks.pop_front();
				this->queueFull.notify_all();
			}
			else
			{
				task = nullptr;
			}
		}
		if (task != nullptr)
		{
			(*task)();
			delete task;
		}
		else
		{
			if (this->state.load() == Stopping)
			{
				this->queueFull.notify_all();
				break;
			}
		}
	}
}

bool BlockingTaskQueue::Add(Task* task)
{
	if (this->state.load() == Stopping || this->state.load() == Stopped)
		return false;

	std::unique_lock<std::mutex> flk(this->fullQueueMutex);
	this->queueFull.wait(flk, [=]() { return this->tasks.size() < this->queueSize; });
	{
		std::lock_guard<std::mutex> lk(this->tasksMutex);
		this->tasks.push_back(task);

	}
	this->cv.notify_all();

	return true;
}

void BlockingTaskQueue::Drain()
{
	this->state.compare_exchange_strong(Running, Stopping);
	{
		std::unique_lock<std::mutex> flk(this->tasksMutex);
		this->queueFull.wait(flk, [=]() { return this->tasks.size() == 0; });
	}

	this->cv.notify_all();
	for (vector<thread*>::iterator iter = this->threads.begin(); iter != this->threads.end(); ++iter)
	{
		auto t = (*iter);
		t->join();
	}
	this->state.exchange(Stopped);
}
