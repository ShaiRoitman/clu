#include "NumberOperator.h"
#include "CommandLineHandlers\SingleCommandLineHandler.h"
#include <boost/regex.hpp>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class AccumulatedCountOperator : public InputFileOperator
{
public:
	AccumulatedCountOperator(string regex)
	{
		m_regex = boost::regex(regex);
	}

	virtual bool OnLineRead(string& line)
	{
		boost::match_results<string::const_iterator> matches;
		if (boost::regex_match(line, matches, m_regex))
		{
			string key = matches["Key"];
			string display = matches["Display"];

			if (m_CurrentKeyValues.find(key) == m_CurrentKeyValues.end())
			{
				m_CurrentKeyValues[key] = 1;
			}
			else
			{
				m_CurrentKeyValues[key]++;
			}

			m_OutputHandler->Output(key);
			m_OutputHandler->Output(" : ");
			m_OutputHandler->OutputDoubleNoLineFeed(m_CurrentKeyValues[key]);
			m_OutputHandler->Output(" : ");
			m_OutputHandler->OutputLineFeed(display);
		}

		return true;
	}

	virtual void OnEnd()
	{
		m_CurrentKeyValues.clear();
	}

private:
	boost::regex m_regex;
	map<string, double> m_CurrentKeyValues;
};

REGISTER_SINGLE_STRING("accumulatedcount", AccumulatedCountOperator)->SetHelp("Prints Display accumulated count of Key");
