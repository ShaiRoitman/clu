#include "Utils.h"
#include "InputFileOperator.h"
#include "CommandLineHandlers\PredicateCommandLineHandler.h"
#include <string>
#include <map>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class HistogramOperator : public InputFileOperator {
public:
	virtual bool OnLineRead(string& input)
	{
		m_instanceCount[input]++;
		return true;
	}
	virtual void OnEnd()
	{
		for (map <string, int>::iterator iter = m_instanceCount.begin();
		iter != m_instanceCount.end();
			++iter)
		{
			const string& value = (*iter).first;
			int& count = (*iter).second;
			this->m_OutputHandler->OutputLineFeed(StringValue(count) + " : [" + value + "]");
		}

		m_instanceCount.clear();
	}
protected:
	map <string, int> m_instanceCount;
};

REGISTER_PREDICATE("histogram", HistogramOperator)->SetHelp("Show Histogram");
