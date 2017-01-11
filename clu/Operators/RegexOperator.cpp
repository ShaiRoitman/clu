#include "InputFileOperator.h"
#include "CommandLineHandlers\SingleCommandLineHandler.h"
#include <boost/regex.hpp>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class RegexOperator : public InputFileOperator
{
public:
	RegexOperator(string regex)
	{
		this->delimitString = GetEnvironmentOrDefault("CLU_REGEX_DELIM", ",");

		m_regex = boost::regex(regex.c_str());
	}

	virtual bool OnLineRead(string& line)
	{
		boost::match_results<string::const_iterator> matches;
		if (boost::regex_match(line, matches, m_regex))
		{
			for (size_t i = 1; i < matches.size(); i++)
			{
				m_OutputHandler->Output(matches[i]);
				if (i != (matches.size() - 1))
				{
					m_OutputHandler->Output(this->delimitString);
				}
			}
			m_OutputHandler->OutputLineFeed("");
		}

		return true;
	}

private:
	string delimitString;
	boost::regex m_regex;
};

REGISTER_SINGLE_STRING("regex", RegexOperator)->SetHelp("Extracts the groups of the regular expression, use , or CLU_REGEX_DELIM between groups");

