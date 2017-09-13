#include "InputFileOperator.h"
#include "CommandLineHandlers/SingleCommandLineHandler.h"
#include <thread>
#include <chrono>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class TimeEndToStart : public InputFileOperator {
public:
	TimeEndToStart(int period)
	{
		this->duration = std::chrono::milliseconds(period);
	}

	virtual bool OnLineRead(string& line)
	{
		this_thread::sleep_for(this->duration);
		m_OutputHandler->OutputLineFeed(line);
		return true;
	}
private:
	std::chrono::milliseconds duration;
};

REGISTER_SINGLE_INTEGER("timeEndToStart", TimeEndToStart)->SetHelp("Delay the input from end to Start");
