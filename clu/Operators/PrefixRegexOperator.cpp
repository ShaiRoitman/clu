#include "InputFileOperator.h"
#include "CommandLineHandlers\SingleCommandLineHandler.h"
#include <boost/regex.hpp>
#include "NumberOperator.h"
#include <boost/algorithm/string/trim.hpp>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class PrefixRegexOperator : public InputFileOperator
{
public:
	PrefixRegexOperator(string regex)
	{
		m_regex = boost::regex(regex);
		currentPrefix = "";
	}

	virtual bool OnLineRead(string& line)
	{
		boost::match_results<string::const_iterator> matches;
		if (boost::regex_match(line, matches, m_regex))
		{
			currentPrefix = matches[1];
		}
		else
		{
			m_OutputHandler->Output(currentPrefix);
			m_OutputHandler->Output(",");
			m_OutputHandler->OutputLineFeed(line);
		}
		return true;
	}

	virtual void OnEnd()
	{
		currentPrefix = "";
	}

	boost::regex m_regex;
	string currentPrefix;
};

REGISTER_SINGLE_STRING("prefixregex", PrefixRegexOperator)->SetHelp("Add the prefix regex to evey following line");
