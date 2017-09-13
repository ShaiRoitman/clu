#include "AggregateOperator.h"
#include "CommandLineHandlers/PredicateCommandLineHandler.h"

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class Counter : public InputFileOperator
{
public:
	Counter() : InputFileOperator(), m_Counter(1) {}
	virtual bool OnLineRead(string& input)
	{
		m_OutputHandler->OutputLineFeed(StringValue(m_Counter) + input);
		m_Counter++;
		return true;
	}

	virtual void OnEnd()
	{
		m_Counter = 1;
	}

protected:
	int m_Counter;
};

REGISTER_PREDICATE("countlines", Counter)->SetHelp("Enumerates the number of lines");
