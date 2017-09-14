#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <deque>
#include <mutex>
#include <atomic>
#include <vector>
#include <condition_variable>
#include <thread>

#include "Task.h"

class BlockingTaskQueue
{
public:
	BlockingTaskQueue(int queueSize);
	void Start(int numberOfThreads);
	void WorkerThread();
	bool Add(Task* task);
	void Drain();

	std::deque<Task*> tasks;
	std::vector<std::thread*> threads;
	std::mutex tasksMutex;
	std::mutex fullQueueMutex;
	std::condition_variable cv;
	std::condition_variable queueFull;
	std::atomic<int> state;
	int queueSize;
};
