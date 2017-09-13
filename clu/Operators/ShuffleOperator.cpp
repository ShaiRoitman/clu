#include "FullFileOperator.h"
#include "CommandLineHandlers/PredicateCommandLineHandler.h"

#include <algorithm>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class ShuffleOperator : public FullFileOperator
{
	virtual void OnEnd()
	{
		random_shuffle(m_data.begin(), m_data.end());
		PrintData();

		FullFileOperator::OnEnd();
	}
};

REGISTER_PREDICATE("shuffle", ShuffleOperator)->SetHelp("Shuffle randomly the input stream");
