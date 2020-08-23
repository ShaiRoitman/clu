#pragma once

#include "NumberOperator.h"

class AggregateOperator : public NumberOperator
{
public:
	AggregateOperator() : m_value(0) {}
	virtual void OnEnd();
protected:
	double m_value;
};
