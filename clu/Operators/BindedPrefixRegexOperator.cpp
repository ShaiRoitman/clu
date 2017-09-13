#include "InputFileOperator.h"
#include "CommandLineHandlers/TwoStringCommandLineHandler.h"
#include <boost/regex.hpp>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class BindedPrefixRegexOperator : public InputFileOperator
{
public:
	BindedPrefixRegexOperator(string binder, string regex)
	{
		m_regex = boost::regex(regex);
		m_binder = boost::regex(binder);
	}

	virtual bool OnLineRead(string& line)
	{
		boost::match_results<string::const_iterator> binder_matches;
		if (boost::regex_match(line, binder_matches, m_binder))
		{
			boost::match_results<string::const_iterator> matches;
			if (boost::regex_match(line, matches, m_regex))
			{
				string key = binder_matches[1].str();
				string value = matches[1].str();
				m_prefix[key] = value;
			}
			else
			{
				if (m_prefix.find(binder_matches[1]) != m_prefix.end())
				{
					m_OutputHandler->Output(m_prefix[binder_matches[1]]);
				}
				m_OutputHandler->Output(",");
				m_OutputHandler->OutputLineFeed(line);
			}
		}

		return true;
	}

	virtual void OnEnd()
	{
		m_prefix.clear();
	}

private:
	boost::regex m_regex;
	boost::regex m_binder;
	map<string, string> m_prefix;
};

//cmd_handler.AddCommandLineHandler(new TwoStringCommandLineHandler<BindedPrefixRegexOperator>("bind", "prefix", "Binds regular expression", "Prefix for the bounded regular expression"));
