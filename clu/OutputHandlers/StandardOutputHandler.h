#pragma once

#include "OutputHandler.h"

class StandardOutputHandler : public OutputHandler
{
public:
	virtual bool Output(const std::string& str);
	virtual bool OutputInteger(int value);
	virtual bool OutputDoubleNoLineFeed(double value);
	virtual bool OutputLong(long value);
};

