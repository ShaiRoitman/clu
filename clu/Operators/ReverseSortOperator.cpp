#include "FullFileOperator.h"
#include "CommandLineHandlers\PredicateCommandLineHandler.h"

USING_NAMESPACE(std);
USING_NAMESPACE(clu);


class ReverseSortOperator : public FullFileOperator
{
public:
	virtual void OnEnd()
	{
		sort(m_data.begin(), m_data.end());
		PrintReverseData();

		FullFileOperator::OnEnd();
	}
};

REGISTER_PREDICATE("rsort", ReverseSortOperator)->SetHelp("Sorts the data in reverse");
