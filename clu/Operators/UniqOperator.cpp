#include "InputFileOperator.h"
#include "CommandLineHandlers/PredicateCommandLineHandler.h"

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class UniqOperator : public InputFileOperator
{
public:
	virtual bool OnLineRead(string& input)
	{
		if (m_instances.find(input) == m_instances.end())
		{
			m_instances.insert(input);
			m_OutputHandler->OutputLineFeed(input);
		}

		return true;
	}

	virtual void OnEnd()
	{
		m_instances.clear();
	}

private:
	set <string> m_instances;
};

REGISTER_PREDICATE("uniq", UniqOperator)->SetHelp("Prints each uniq line once");
