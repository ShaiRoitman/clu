#include "InputFileOperator.h"
#include "CommandLineHandlers\SingleCommandLineHandler.h"
#include <boost/regex.hpp>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class FormatStringOperator : public InputFileOperator {
public:
	FormatStringOperator(string format) : m_format(format)
	{
		std::string searchRegex = GetEnvironmentOrDefault("CLU_FORMAT", "%%1");
		m_search_regex = boost::regex(searchRegex);
	}
	virtual bool OnLineRead(string& line)
	{
		string ret_value = boost::regex_replace(m_format + "\n", m_search_regex, line, boost::regex_constants::format_literal);
		m_OutputHandler->Output(ret_value);
		return true;
	}
protected:
	string m_format;
	boost::regex m_search_regex;
};

REGISTER_SINGLE_STRING("format", FormatStringOperator)->SetHelp("Format line - Use %%1 to replace the current string, CLU_FORMAT to replace %%1");