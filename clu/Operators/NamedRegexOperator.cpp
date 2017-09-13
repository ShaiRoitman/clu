#include "InputFileOperator.h"
#include "CommandLineHandlers/CommandLineHandler.h"

#include <boost/tokenizer.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/program_options.hpp>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class NamedRegexOperator : public InputFileOperator {
public:
	NamedRegexOperator(const string& regex, const string& format) : m_format(format)
	{
		const char formatRegex[] = "%%([a-zA-Z0-9]+)";
		m_format_regex = boost::regex(formatRegex);
		if (regex.length() > 0)
			m_regex = boost::regex(regex.c_str(), boost::regex_constants::ECMAScript);
	}
	virtual bool OnLineRead(string& line)
	{
		boost::match_results<string::const_iterator> matches;
		if (boost::regex_match(line, matches, m_regex)) {
			boost::match_results<string::const_iterator> formatMatches;
			string newValue = m_format;
			while (boost::regex_search(newValue, formatMatches, m_format_regex)) {
				string name = formatMatches[1];
				string value = "[" + name + "]";//matches[name];
				newValue = boost::regex_replace(newValue, boost::regex("%%" + name), value);
			}
			m_OutputHandler->OutputLineFeed(newValue);
		}
		return true;
	}
protected:
	boost::regex m_regex;
	boost::regex m_format_regex;
	string m_format;
};

class NamedRegex_CommandLine : public InputFileOperator_CommandLine
{
	virtual bool AddProgramOptions(boost::program_options::options_description& desc)
	{
		desc.add_options()
			("namedregex", boost::program_options::value<string>(), "Named Regex Operator - Regular expression including named capturing groups")
			("namedformat", boost::program_options::value<string>(), "Named Regex Operator - Format expression including names of groups from the regex in %%groupname format");
		return true;
	}
	virtual InputFileOperator* GetOperator(const boost::program_options::variables_map &vm)
	{
		InputFileOperator* ret_value = NULL;
		if (vm.count("namedregex") && vm.count("namedformat"))
		{
			string regex = vm["namedregex"].as<string>();
			string format = vm["namedformat"].as<string>();
			ret_value = new NamedRegexOperator(regex, format);
		}
		return ret_value;
	}
};
static auto nameRegisterVar = CommandLineHandler::GetInstance().AddCommandLineHandler(new NamedRegex_CommandLine());
