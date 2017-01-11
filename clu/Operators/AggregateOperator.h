#pragma once

#include "NumberOperator.h"

class AggregateOperator : public NumberOperator
{
public:
	virtual void OnEnd();
protected:
	double m_value;
};
