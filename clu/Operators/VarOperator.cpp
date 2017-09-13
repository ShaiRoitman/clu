#include "VarOperator.h"
#include "CommandLineHandlers/PredicateCommandLineHandler.h"

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

bool VarOperator::OnStart()
{
	m_sqrsum = 0;
	m_sum = 0;
	m_count = 0;
	return true;
}
	bool VarOperator::OnNumberRead(double number)
{
	m_count++;
	m_sqrsum += number*number;
	m_sum += number;
	return true;
}
void VarOperator::OnEnd()
{
	double total = 0.0;
	if (m_count > 0)
		total = (m_sqrsum - (m_sum * m_sum) / m_count) / m_count;

	m_OutputHandler->OutputDouble(total);
	m_sqrsum = 0;
	m_sum = 0;
	m_count = 0;
	AggregateOperator::OnEnd();
}

REGISTER_PREDICATE("var", VarOperator)->SetHelp("Calculate Variance");
