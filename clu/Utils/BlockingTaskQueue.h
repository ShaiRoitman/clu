#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <deque>
#include <mutex>
#include <atomic>
#include <vector>

#include "Task.h"

using namespace std::chrono_literals;
using namespace std;

class BlockingTaskQueue
{
public:
	BlockingTaskQueue(int queueSize);
	void Start(int numberOfThreads);
	void WorkerThread();
	bool BlockingTaskQueue::Add(Task* task);
	void BlockingTaskQueue::Drain();

	deque<Task*> tasks;
	vector<thread*> threads;
	std::mutex tasksMutex;
	std::mutex fullQueueMutex;
	std::condition_variable cv;
	std::condition_variable queueFull;
	std::atomic<int> state;
	int queueSize;
};
