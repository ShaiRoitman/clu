#include "AggregateOperator.h"
#include "CommandLineHandlers\PredicateCommandLineHandler.h"
#include "Globals.h"

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class HelpOperator : public InputFileOperator {
public:
	HelpOperator() {}
	virtual bool OnLineRead(string& line) { return false; }
	virtual bool OnStart()
	{
		auto desc = CommandLineHandler::GetInstance().GetOptionsDescriptions();
		cout << string("clu ( Command Line Utility ) : ") + g_version + string("\n");
		cout << string("Written by ") << string(g_developer_name) + string("\n\n");
		cout << desc << "\n";
		return false;
	}
};

REGISTER_PREDICATE("help", HelpOperator)->SetHelp("Show help");
