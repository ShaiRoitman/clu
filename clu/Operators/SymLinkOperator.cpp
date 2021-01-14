#include "InputFileOperator.h"
#include "CommandLineHandlers/PredicateCommandLineHandler.h"

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class SymLinkOperator : public clu::InputFileOperator {
public:
	SymLinkOperator()
	{
		m_regex = "(.*),(.*)";
	}
	bool OnLineRead(string& line)
	{
		boost::filesystem::path source;
		boost::filesystem::path target;

		boost::match_results<string::const_iterator> matches;
		if (boost::regex_match(line, matches, m_regex))
		{
			source = matches[1].str();
			target = matches[2].str();
			boost::system::error_code ec;
			boost::filesystem::create_symlink(source, target, ec);
			if (ec.value() != 0)
			{
				m_OutputHandler->Output(string("failed to create link source [") + source.string() + string("] target [") + target.string() + string("]\n") );
			}
		}

		return true;
	}
private:
	boost::regex m_regex;
};

REGISTER_PREDICATE("symlink", SymLinkOperator)->SetHelp("Create symbolic links from source to target");
