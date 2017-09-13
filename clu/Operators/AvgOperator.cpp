#include "AggregateOperator.h"
#include "CommandLineHandlers/PredicateCommandLineHandler.h"

class AvgOperator : public AggregateOperator {
public:
	virtual bool OnStart()
	{
		m_value = 0;
		m_count = 0;
		return true;
	}
	virtual bool OnNumberRead(double number)
	{
		m_count++;
		m_value += number;
		return true;
	}
	virtual void OnEnd()
	{
		if (m_count > 0)
			m_OutputHandler->OutputDouble(m_value / m_count);
		else
			m_OutputHandler->OutputDouble(0.0);

		m_count = 0;
		AggregateOperator::OnEnd();
	}
protected:
	int m_count;
};

REGISTER_PREDICATE("avg", AvgOperator)->SetHelp("Calculate Average");
