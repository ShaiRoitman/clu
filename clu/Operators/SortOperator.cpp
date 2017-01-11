#include "FullFileOperator.h"
#include "CommandLineHandlers\PredicateCommandLineHandler.h"

USING_NAMESPACE(std);
USING_NAMESPACE(clu);


class SortOperator : public FullFileOperator
{
public:
	virtual void OnEnd()
	{
		sort(m_data.begin(), m_data.end());
		PrintData();
		FullFileOperator::OnEnd();
	}
};

REGISTER_PREDICATE("sort", SortOperator)->SetHelp("Sorts the data");
