#include "AggregateOperator.h"
#include "CommandLineHandlers\PredicateCommandLineHandler.h"
#include "Globals.h"

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class VersionOperator : public InputFileOperator {
public:
	VersionOperator() {};
	virtual bool OnLineRead(string& line) { return false; }
	virtual bool OnStart()
	{
		m_OutputHandler->OutputLineFeed(string("clu ( Command Line Utility ) : ") + g_version);
		return false;
	}
};


REGISTER_PREDICATE("version", VersionOperator)->SetHelp("Show Version");
