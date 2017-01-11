#include "FullFileOperator.h"
#include "CommandLineHandlers\PredicateCommandLineHandler.h"

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class ReverseOperator : public FullFileOperator
{
public:
	virtual void OnEnd()
	{
		PrintReverseData();
		FullFileOperator::OnEnd();
	}
};

REGISTER_PREDICATE("reverse", ReverseOperator)->SetHelp("reverse the input stream");
