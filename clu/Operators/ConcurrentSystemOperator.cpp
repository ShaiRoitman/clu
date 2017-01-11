#include "Utils.h"
#include "InputFileOperator.h"
#include "CommandLineHandlers\SingleCommandLineHandler.h"
#include <boost/algorithm/string/trim.hpp>

#include "Utils\Task.h"
#include "Utils\ProcessingQueue.h"

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class ExecuteTask : public Task
{
public:
	ExecuteTask(const string& cmd) :
		m_cmd(cmd)
	{
	}

	virtual void operator()()
	{
		system(this->m_cmd.c_str());
	}

private:
	string m_cmd;
};

class ConcurrentSystemOperator : public InputFileOperator
{
public:
	ConcurrentSystemOperator(int count)
	{
		this->taskManager = new ProcessingQueue(GetEnvironmentOrDefaultInteger("CLU_SYSTEM_CONCURRENT", count), GetEnvironmentOrDefaultInteger("CLU_SYSTEM_DELAY", 0));
		this->taskManager->Start();
	}

	virtual bool OnLineRead(string& line)
	{
		string cmd = line;
		boost::algorithm::trim(cmd);
		if (!cmd.empty())
		{
			shared_ptr<Task> task(new ExecuteTask(cmd));
			this->taskManager->Add(task);
		}

		return true;
	}

	virtual void OnEnd()
	{
		this->taskManager->Stop();
		this->taskManager->WaitForCompletion();
	}

private:
	ProcessingQueue* taskManager;
};


REGISTER_SINGLE_INTEGER("csystem", ConcurrentSystemOperator)->SetHelp("Executes each input in system with concurrency level");
