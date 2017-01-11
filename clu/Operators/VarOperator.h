#pragma once

#include "AggregateOperator.h"

class VarOperator : public AggregateOperator
{
public:
	virtual bool OnStart();
	virtual bool OnNumberRead(double number);
	virtual void OnEnd();
protected:
	int m_count;
	double m_sum;
	double m_sqrsum;
};
