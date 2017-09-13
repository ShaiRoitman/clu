#include "InputFileOperator.h"
#include "CommandLineHandlers/SingleCommandLineHandler.h"

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class HeadOperator : public InputFileOperator
{
public:
	HeadOperator(int headCount) : m_maxcount(headCount), m_count(0) {};
	virtual bool OnLineRead(string& line)
	{
		m_count++;
		if (m_count <= m_maxcount)
		{
			m_OutputHandler->OutputLineFeed(line);
			return true;
		}
		else
			return false;
	}

	virtual void OnEnd()
	{
		m_count = 0;
	}

protected:
	size_t m_count;
	size_t m_maxcount;
};

REGISTER_SINGLE_INTEGER("head", HeadOperator)->SetHelp("Prints the top lines");
