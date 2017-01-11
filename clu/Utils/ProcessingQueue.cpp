#include "ProcessingQueue.h"
#include <iostream>

// Processing Queue
static int Created = 0;
static int Running = 1;
static int Stopping = 2;
static int Stopped = 3;

ProcessingQueue::ProcessingQueue(int concerrencyLevel, int timeToWaitInMs) :
	concerrencyLevel(concerrencyLevel), status(Created), countdown(concerrencyLevel), timeToWaitInMs(timeToWaitInMs)
{
}

bool ProcessingQueue::SetRunning()
{
	return this->status.compare_exchange_strong(Created, Running);
}

bool ProcessingQueue::SetStopping()
{
	return this->status.compare_exchange_strong(Running, Stopping);
}

bool ProcessingQueue::SetStopped()
{
	return this->status.compare_exchange_strong(Stopping, Stopped);
}

void ProcessingQueue::Start()
{
	if (this->SetRunning())
	{
		for (int i = 0; i < this->concerrencyLevel; i++)
		{
			this->threads.push_back(thread([=] { WorkerThread(); }));
		}
	}
}

void ProcessingQueue::Stop()
{
	std::lock_guard<std::mutex> guard(this->m_mutex);
	this->SetStopping();
}

bool ProcessingQueue::Add(shared_ptr<Task> task)
{
	std::lock_guard<std::mutex> guard(this->m_mutex);
	int status = this->status.load();

	if (status == Running)
	{
		this->workItems.Enqueue(task);
		return true;
	}

	return false;
}

void ProcessingQueue::WaitForCompletion()
{
	if (this->status.load() == Created)
	{
		return;
	}

	this->countdown.Wait();

}

void ProcessingQueue::WorkerThread()
{
	if (this->status.load() == Created)
	{
		return;
	}

	while (this->status.load() != Stopped)
	{
		shared_ptr<Task> workItem;
		if (this->workItems.TryTake(workItem, 100))
		{
			(*workItem.get())();
		}
		else
		{
			if (this->status.load() == Stopping)
			{
				if (this->countdown.Signal())
				{
					this->SetStopped();
				}

				// END OF WORK
				return;
			}
		}
	}
}
// End of ProcessingQueue