#include "AggregateOperator.h"
#include "CommandLineHandlers\PredicateCommandLineHandler.h"
#include <limits>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class MaxOperator : public AggregateOperator {
public:
	virtual bool OnStart()
	{
		m_value = numeric_limits<double>::min();
		return true;
	}
	virtual bool OnNumberRead(double number)
	{
		if (m_value < number)
			m_value = number;
		return true;
	}

	virtual void OnEnd()
	{
		m_OutputHandler->OutputDouble(m_value);
		AggregateOperator::OnEnd();
	}

};

REGISTER_PREDICATE("max", MaxOperator)->SetHelp("Find Maximum");
