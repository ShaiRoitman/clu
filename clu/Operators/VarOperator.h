#pragma once

#include "AggregateOperator.h"

class VarOperator : public AggregateOperator
{
public:
	VarOperator() : m_count(0), m_sum(0), m_sqrsum(0) {}
	virtual bool OnStart();
	virtual bool OnNumberRead(double number);
	virtual void OnEnd();
protected:
	int m_count;
	double m_sum;
	double m_sqrsum;
};
