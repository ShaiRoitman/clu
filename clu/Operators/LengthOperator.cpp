#include "AggregateOperator.h"
#include "CommandLineHandlers/PredicateCommandLineHandler.h"

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class LengthOperator : public InputFileOperator
{
public:
	LengthOperator() {};
	virtual bool OnLineRead(string& input)
	{
		m_OutputHandler->OutputInteger((int)input.length());
		return true;
	}
};

REGISTER_PREDICATE("linelength", LengthOperator)->SetHelp("Output the length of the line");
