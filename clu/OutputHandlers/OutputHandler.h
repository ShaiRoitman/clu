#pragma once

#include <string>
#include <list>
#include <iostream>
#include <fstream>

class OutputHandler {
public:
	virtual ~OutputHandler();
	virtual bool OnStartHandling();
	virtual bool OnEndHandling();
	virtual bool OutputDouble(double value);
	virtual bool OutputLineFeed(const std::string& str);

	virtual bool Output(const std::string& str) = 0;
	virtual bool OutputInteger(int value) = 0;
	virtual bool OutputLong(long value) = 0;
	virtual bool OutputDoubleNoLineFeed(double value) = 0;
};
