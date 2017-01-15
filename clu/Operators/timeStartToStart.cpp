#include "InputFileOperator.h"
#include "CommandLineHandlers\SingleCommandLineHandler.h"
#include <thread>
#include <chrono>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class TimeStartToStart : public InputFileOperator {
public:
	TimeStartToStart(int period)
	{
		this->duration = std::chrono::milliseconds(period);
		this->lastTime = std::chrono::system_clock::now();
	}

	virtual bool OnLineRead(string& line)
	{
		auto now = std::chrono::system_clock::now();
		auto diff = now - this->lastTime;
		if (diff < this->duration)
		{
			auto waitPeriod = this->duration - diff;
			this_thread::sleep_for(waitPeriod);
		}
		this->lastTime = std::chrono::system_clock::now();
		m_OutputHandler->OutputLineFeed(line);
		return true;
	}

private:
	std::chrono::milliseconds duration;
	std::chrono::time_point<std::chrono::system_clock> lastTime;
};

REGISTER_SINGLE_INTEGER("timeStartToStart", TimeStartToStart)->SetHelp("Delay the input from start to start");
