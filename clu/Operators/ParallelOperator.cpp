#include "InputFileOperator.h"
#include "RegexOperator.h"
#include "Task.h"
#include "MemoryOutputHandler.h"
#include <queue>
#include <mutex>
#include "CommandLineHandlers\SingleCommandLineHandler.h"
#include "BlockingTaskQueue.h"

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class Container
{
public:
	Container(long id, InputFileOperator* handler, int size)
	{
		this->id = id;
		this->data.reserve(size);
		this->outputHandler = new MemoryOutputHandler();
		this->handler = handler;
		this->handler->m_OutputHandler = this->outputHandler;
	}

	void DoWork()
	{
		handler->m_OutputHandler = outputHandler;

		handler->OnStart();
		for (auto iter = data.begin(); iter != data.end(); ++iter)
		{
			string& line = (*iter);
			handler->OnLineRead(line);
		}
		handler->OnEnd();
	}

	vector<string> data;
	InputFileOperator* handler;
	MemoryOutputHandler* outputHandler;
	long id;
};

class LessThanById
{
public:
	bool operator()(Container*& lhs, Container*& rhs) const
	{
		return lhs->id < rhs->id;
	}
};

class MergeContainer
{
public:
	MergeContainer(OutputHandler *handler)
	{
		this->handler = handler;
		this->nextId = 1;
		this->lastId.store(LONG_MAX);
		this->workerThread = new std::thread([=]() { this->WorkerThread(); });
	}

	void WorkerThread()
	{
		while (this->nextId != this->lastId.load())
		{
			std::unique_lock<std::mutex> lk(this->queue_mutex);
			this->hasWork.wait(lk, [=]()
			{
				auto retValue = this->containerQueue.size() > 0 || this->lastId.load() != LONG_MAX;
				return retValue;
			});

			while (this->containerQueue.size() > 0 && containerQueue.top()->id == nextId)
			{
				auto top = this->containerQueue.top();
				this->containerQueue.pop();
				printf("Output Container %d\n", top->id);
				this->Output(top->outputHandler);
				delete top;
				this->nextId++;
			}

			if (this->nextId > this->lastId.load())
				return;

		}
	}

	void Add(Container* container)
	{
		{
			std::lock_guard<std::mutex> guard(this->queue_mutex);
			containerQueue.push(container);
			printf("Adding Container Id %d\n", container->id);
		}
		hasWork.notify_all();
	}

	void WaitForId(long id)
	{
		{
			std::lock_guard<std::mutex> guard(this->queue_mutex);
			this->lastId.store(id);
			printf("Waiting for Id %d\n", id);
		}
		hasWork.notify_all();

		this->workerThread->join();
	}

	void Output(MemoryOutputHandler* output)
	{
		for (std::list<std::string>::iterator iter = output->m_Lines.begin(); iter != output->m_Lines.end(); ++iter)
		{
			//this->handler->Output(*iter);
		}
	}

public:
	OutputHandler *handler;
	priority_queue<Container*, std::vector<Container*>, LessThanById> containerQueue;
	long nextId;
	std::atomic<long> lastId;
	std::mutex queue_mutex;
	std::thread* workerThread;
	std::condition_variable hasWork;
};

class OperateAndMergeTask : public Task
{
public:
	OperateAndMergeTask(Container* container, MergeContainer* mergeContainer)
	{
		this->container = container;
		this->mergeContainer = mergeContainer;
	}

	virtual void operator()()
	{
		container->DoWork();
		mergeContainer->Add(container);
	}

public:
	Container* container;
	MergeContainer* mergeContainer;
};

class ParallelOperator : public InputFileOperator
{
public:
	ParallelOperator(int countSize, int numberOfThreads, int queueSize, std::function<InputFileOperator*()> operatorFactory)
	{
		this->regex = regex;
		this->countSize = countSize;
		this->numberOfThreads = numberOfThreads;
		this->maxQueueSize = queueSize;
		this->lastId = 1;
		this->currentContainer = nullptr;
		this->mergeContainer = nullptr;
		this->taskQueue = nullptr;
		this->operatorFactory = operatorFactory;
	}

	virtual InputFileOperator* CreateNewOperator()
	{
		return this->operatorFactory();
	}

	Container* CreateNewContainer()
	{
		Container* retValue = new Container(this->lastId, this->CreateNewOperator(), this->countSize);
		this->lastId++;
		return retValue;
	}

	virtual bool OnStart()
	{
		this->currentContainer = CreateNewContainer();
		this->mergeContainer = new MergeContainer(this->m_OutputHandler);
		this->taskQueue = new BlockingTaskQueue(this->maxQueueSize);
		this->taskQueue->Start(this->numberOfThreads);
		return true;
	}

	virtual bool OnLineRead(string& line)
	{
		this->currentContainer->data.push_back(line);
		if (this->currentContainer->data.size() == this->countSize)
		{
			taskQueue->Add(new OperateAndMergeTask(this->currentContainer, this->mergeContainer));
			this->currentContainer = CreateNewContainer();
		}

		return true;
	}

	virtual void OnEnd() override
	{
		taskQueue->Add(new OperateAndMergeTask(this->currentContainer, this->mergeContainer));
		this->currentContainer = nullptr;
		this->taskQueue->Drain();
		this->mergeContainer->WaitForId(this->lastId - 1);
	}

private:
	Container* currentContainer;
	MergeContainer* mergeContainer;
	BlockingTaskQueue* taskQueue;
	string regex;
	long lastId;
	int countSize;
	int numberOfThreads;
	int maxQueueSize;

	std::function<InputFileOperator*()>  operatorFactory;
};

clu::InputFileOperator* CreateParallelOperator(
	int countSize, 
	int numberOfThreads, 
	int queueSize, 
	std::function<clu::InputFileOperator*()> operatorFactory)
{
	return new ParallelOperator(countSize, numberOfThreads, queueSize, operatorFactory);
}
