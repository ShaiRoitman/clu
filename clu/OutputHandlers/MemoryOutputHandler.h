#pragma once

#include "OutputHandler.h"

class MemoryOutputHandler : public OutputHandler
{
public:
	MemoryOutputHandler();
	virtual bool Output(const std::string& str);
	virtual bool OutputInteger(int value);
	virtual bool OutputDoubleNoLineFeed(double value);
	virtual bool OutputLong(long value);
	virtual bool OnStartHandling();
	virtual bool OnEndHandling();

public:
	std::list<std::string> m_Lines;
};
