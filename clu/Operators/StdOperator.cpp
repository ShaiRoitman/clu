#include "VarOperator.h"

class StdOperator : public VarOperator {
public:
	virtual void OnEnd()
	{
		double total = 0.0;
		if (m_count > 0)
			total = (m_sqrsum - (m_sum * m_sum) / m_count) / m_count;
		total = sqrt(total);

		m_OutputHandler->OutputDouble(total);
	}
};

REGISTER_PREDICATE("stddev", StdOperator)->SetHelp("Calculate Standard Deviation");
