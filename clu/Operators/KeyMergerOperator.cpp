#include "InputFileOperator.h"
#include "CommandLineHandlers/SingleCommandLineHandler.h"
#include <boost/regex.hpp>
#include "NumberOperator.h"
#include <boost/algorithm/string/trim.hpp>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class KeyMerger : public InputFileOperator
{
public:
	KeyMerger(const string& dataDelimiter)
		: m_KeyDelimiter(','), m_DataDelimiter(dataDelimiter) {}
	virtual bool OnLineRead(string& input)
	{
		size_t delimPosition = input.find(m_KeyDelimiter);
		if (delimPosition != string::npos)
		{
			string key = input.substr(0, delimPosition);
			string value = input.substr(delimPosition + 1);
			if (m_Keys.find(key) != m_Keys.end())
			{
				m_Keys[key] = m_Keys[key] + m_DataDelimiter + value;
			}
			else
			{
				m_Keys[key] = value;
				m_OrderKeys.push_back(key);
			}
		}
		return true;
	}

	virtual void OnEnd()
	{
		for (list<string>::iterator iter = m_OrderKeys.begin();
		iter != m_OrderKeys.end();
			++iter)
		{
			m_OutputHandler->Output("[");
			m_OutputHandler->Output((*iter));
			m_OutputHandler->Output("] : [");
			m_OutputHandler->Output(m_Keys[(*iter)]);
			m_OutputHandler->Output("]\n");
		}

		m_Keys.clear();
		m_OrderKeys.clear();
	}

protected:
	map<string, string> m_Keys;
	list<string> m_OrderKeys;
	char m_KeyDelimiter;
	string m_DataDelimiter;
};

REGISTER_SINGLE_STRING("keymerge", KeyMerger)->SetHelp("Merge lines by the first element as key");
