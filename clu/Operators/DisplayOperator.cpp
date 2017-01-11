#include "AggregateOperator.h"
#include "CommandLineHandlers\PredicateCommandLineHandler.h"

USING_NAMESPACE(std);
USING_NAMESPACE(clu); 

class DisplayOperator : public InputFileOperator {
public:
	DisplayOperator() {};
	virtual bool OnLineRead(string& line)
	{
		m_OutputHandler->OutputLineFeed(line);
		return true;
	}
};

REGISTER_PREDICATE("display", DisplayOperator)->SetHelp("Show the input");
