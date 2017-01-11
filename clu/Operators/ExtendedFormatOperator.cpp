#include "InputFileOperator.h"
#include "CommandLineHandlers\CommandLineHandler.h"

#include <boost/tokenizer.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/program_options.hpp>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class ExtendedFormatStringOperator : public InputFileOperator {
public:
	ExtendedFormatStringOperator(string format, string delimiters) : m_format(format), m_delimiters(delimiters)
	{
		boost::regex exp("(%%([0-9]+)(.*))");
		boost::match_results<string::const_iterator> matches;
		while (regex_search(format, matches, exp))
		{
			string sIndex = matches[2];
			int index = atoi(sIndex.c_str());
			m_regex[index] = boost::regex(string("%%") + StringValue(index));
			format = matches[3];
		}
	}

	virtual bool OnLineRead(string& line)
	{
		std::vector<string> tokens;
		ExtractTokens(line, tokens, m_delimiters.c_str());
		string ret_value = m_format;

		map<int, boost::regex>::iterator iter;
		for (iter = m_regex.begin(); iter != m_regex.end(); ++iter)
		{
			size_t from_index = (*iter).first;
			boost::regex& search_string = (*iter).second;
			if (from_index <= tokens.size())
			{
				ret_value = boost::regex_replace(ret_value, search_string, tokens[from_index - 1], boost::regex_constants::format_literal);
			}
			else
			{
				ret_value = boost::regex_replace(ret_value, search_string, "", boost::regex_constants::format_literal);
			}
		}

		m_OutputHandler->OutputLineFeed(ret_value);
		return true;
	}
protected:
	string m_format;
	map<int, boost::regex> m_regex;
	string m_delimiters;
};

class ExtendedFormatCommandLineHandler : public InputFileOperator_CommandLine
{
	virtual bool AddProgramOptions(boost::program_options::options_description& desc)
	{
		desc.add_options()
			("eformat", boost::program_options::value<string>(), "Extended Format line - Use %%1 %%2 .. to replace the delimited values")
			("eformatdelimit", boost::program_options::value<string>(), "Extended Format line - default ,");
		return true;
	}

	virtual InputFileOperator* GetOperator(boost::program_options::variables_map &vm)
	{
		InputFileOperator* retValue = NULL;
		string eformatdelimit = ",";
		if (vm.count("eformat") > 0)
		{
			string eformat = vm["eformat"].as<string>();
			if (vm.count("eformatdelimit") > 0) {
				eformatdelimit = vm["eformatdelimit"].as<string>();
			}
			retValue = new ExtendedFormatStringOperator(eformat, eformatdelimit.c_str());
		}

		return retValue;
	}
};

static auto nameRegisterVar = CommandLineHandler::GetInstance().AddCommandLineHandler(new ExtendedFormatCommandLineHandler());
