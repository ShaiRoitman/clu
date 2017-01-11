#include "InputFileOperator.h"
#include "CommandLineHandlers\TwoStringCommandLineHandler.h"
#include "OutputHandlers\FileOutputHandler.h"
#include <boost/regex.hpp>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class SplitFilesOperator : public InputFileOperator
{
public:
	SplitFilesOperator(string regex, string prefix)
	{
		m_regex = boost::regex(regex);
		m_prefix = prefix;
	}

	virtual bool OnLineRead(string& line)
	{
		boost::match_results<string::const_iterator> matches;
		if (boost::regex_match(line, matches, m_regex))
		{
			const string& fileName = matches[1].str();
			if (this->outputHandlers.find(fileName) == this->outputHandlers.end())
			{
				this->outputHandlers[fileName] = new FileOutputHandler(this->m_prefix + fileName);
			}

			this->outputHandlers[fileName]->OutputLineFeed(line);
		}

		return true;
	}

	boost::regex m_regex;
	std::string m_prefix;
	map<string, FileOutputHandler*> outputHandlers;
};

//	cmd_handler.AddCommandLineHandler(new TwoStringCommandLineHandler<SplitFilesOperator>("splitfilesRegex", "splitfilesPrefix", "regex to extract suffix", "Split the input into multiple files with filesPrefix"));
