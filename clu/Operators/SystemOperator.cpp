#include "InputFileOperator.h"
#include "CommandLineHandlers/PredicateCommandLineHandler.h"
#include <boost/algorithm/string/trim.hpp>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class SystemOperator : public InputFileOperator {
public:
	virtual bool OnLineRead(string& line)
	{
		string cmd = line;
		boost::algorithm::trim(cmd);
		if (!cmd.empty())
		{
			if (system(cmd.c_str())) {};
		}

		return true;
	}
};

REGISTER_PREDICATE("system", SystemOperator)->SetHelp("Run the command as system command");
