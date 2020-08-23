#include "AggregateOperator.h"
#include "CommandLineHandlers/SingleCommandLineHandler.h"
#include <ctime>
#include <chrono>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class MeasureTimeOperator : public InputFileOperator {
public:
	MeasureTimeOperator(string operators) : argument(operators), start(0)
	{
		if (this->argument.find("pass") != string::npos)
		{
			this->shouldPass = true;
		}
		else
		{
			this->shouldPass = false;
		}
		
	}

	virtual bool OnLineRead(string& line) 
	{ 
		if (shouldPass)
		{
			m_OutputHandler->OutputLineFeed(line);
		}

		return true; 
	}

	virtual bool OnStart()
	{
		if (this->argument.find("start") != string::npos)
		{
			DisplayCurrentTime();
		}

		this->start = clock();
		return true;
	}

	virtual void OnEnd()
	{
		if (this->argument.find("end") != string::npos)
		{
			DisplayCurrentTime();
		}

		auto diffTime = clock() - start;

		if (this->argument.find("diff") != string::npos)
		{
			m_OutputHandler->Output("Total time in ms :");
			m_OutputHandler->OutputLong(diffTime);
			m_OutputHandler->OutputLineFeed("");
		}
	}

private:
	void DisplayCurrentTime()
	{
		auto now = std::chrono::system_clock::now();
		auto nowTime = std::chrono::system_clock::to_time_t(now);
		auto localTime = std::localtime(&nowTime);

		char mbstr[100];
		std::strftime(mbstr, sizeof(mbstr), "%d/%m/%Y %H:%M:%S", localTime);

		m_OutputHandler->Output("Current Time ");
		m_OutputHandler->OutputLineFeed(mbstr);
	}

	std::clock_t start;
	bool shouldPass;
	string argument;
};

REGISTER_SINGLE_STRING("measureTime", MeasureTimeOperator)->SetHelp("measure Time diff, show time in start,end,diff,pass");
