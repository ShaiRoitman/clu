#include "InputFileOperator.h"
#include "CommandLineHandlers\CommandLineHandler.h"

#include <boost/tokenizer.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/program_options.hpp>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class SearchAndReplace : public InputFileOperator {
public:
	SearchAndReplace(const string& search, const string& replace) : m_search(search), m_replace(replace)
	{
		m_search_regex = boost::regex(m_search.c_str());
	}

	virtual bool OnLineRead(string& line)
	{
		string ret_value = boost::regex_replace(line, m_search_regex, m_replace, boost::regex_constants::format_literal);
		m_OutputHandler->OutputLineFeed(ret_value);
		return true;
	}

	boost::regex m_search_regex;
	string m_search;
	string m_replace;
};

class SearchReplace_CommandLine : public InputFileOperator_CommandLine
{
	virtual bool AddProgramOptions(boost::program_options::options_description& desc)
	{
		desc.add_options()
			("search", boost::program_options::value<string>(), "Search and Replace - Search regex")
			("replace", boost::program_options::value<string>(), "Search and Replace - Replace string");
		return true;
	}

	virtual InputFileOperator* GetOperator(const boost::program_options::variables_map &vm)
	{
		InputFileOperator* ret_value = NULL;
		if (vm.count("search") && vm.count("replace"))
		{
			const string& search = vm["search"].as<string>();
			const string& replace = vm["replace"].as<string>();
			ret_value = new SearchAndReplace(search, replace);
		}
		return ret_value;
	}
};

static auto nameRegisterVar = CommandLineHandler::GetInstance().AddCommandLineHandler(new SearchReplace_CommandLine());
