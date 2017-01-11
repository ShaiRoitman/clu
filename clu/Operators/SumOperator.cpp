#include "AggregateOperator.h"
#include "CommandLineHandlers\PredicateCommandLineHandler.h"

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class SumOperator : public AggregateOperator
{
public:
	virtual bool OnStart()
	{
		m_value = 0;
		return true;
	}
	virtual bool OnNumberRead(double number)
	{
		m_value += number;
		return true;
	}

	virtual void OnEnd()
	{
		m_OutputHandler->OutputDouble(m_value);
		AggregateOperator::OnEnd();
	}
};

REGISTER_PREDICATE("sum", SumOperator)->SetHelp("Calculate Sum");
