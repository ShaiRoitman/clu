#include "InputFileOperator.h"
#include "CommandLineHandlers/SingleCommandLineHandler.h"

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class LineConcat : public InputFileOperator
{
public:
	LineConcat(int count) : m_count(count), m_curcount(0) {};
	virtual bool OnLineRead(string& input)
	{
		m_current += input;
		if (m_curcount == m_count)
		{
			m_OutputHandler->OutputLineFeed(m_current);
			m_current = "";
			m_curcount = 0;
		}
		else
		{
			m_curcount++;
		}
		return true;
	}

	virtual void OnEnd()
	{
		if (m_current.size() != 0)
			m_OutputHandler->OutputLineFeed(m_current);

		m_current = "";
		m_curcount = 0;
	}

protected:
	string m_current;
	int m_count;
	int m_curcount;
};

REGISTER_SINGLE_INTEGER("lconcat", LineConcat)->SetHelp("Appends every n lines into one line");
