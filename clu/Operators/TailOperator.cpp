#include "InputFileOperator.h"
#include "CommandLineHandlers/SingleCommandLineHandler.h"

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class TailOperator : public InputFileOperator
{
public:
	TailOperator(int tailCount) : m_maxcount(tailCount) {};
	virtual bool OnLineRead(string& line)
	{
		if (m_Lines.size() < m_maxcount)
		{
			m_Lines.push_back(line);
		}
		else
		{
			m_Lines.pop_front();
			m_Lines.push_back(line);
		}
		return true;
	}

	virtual void OnEnd()
	{
		list<string>::iterator iter;
		for (iter = m_Lines.begin(); iter != m_Lines.end(); ++iter)
		{
			m_OutputHandler->OutputLineFeed(*iter);
		}
		m_Lines.clear();
	}

protected:
	list<string> m_Lines;
	size_t m_maxcount;
};

REGISTER_SINGLE_INTEGER("tail", TailOperator)->SetHelp("Prints the bottom lines");
