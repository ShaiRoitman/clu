#include "InputFileOperator.h"
#include "CommandLineHandlers\SingleCommandLineHandler.h"

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class EveryOperator : public InputFileOperator {
public:
	EveryOperator(int every) : m_count(0), m_every(every)
	{
		if (m_every == 0)
		{
			m_every = 1;
		}
	}

	virtual bool OnLineRead(string& line)
	{
		if (m_count == 0)
		{
			m_OutputHandler->OutputLineFeed(line);
		}
		m_count = (m_count + 1) % m_every;
		return true;
	}
private:
	int m_count;
	int m_every;
};

REGISTER_SINGLE_INTEGER("every", EveryOperator)->SetHelp("Print every <kth> line");
