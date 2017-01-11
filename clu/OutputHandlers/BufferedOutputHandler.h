#pragma once

#include "OutputHandler.h"

class BufferedOutputHandler : public OutputHandler
{
public:
	BufferedOutputHandler(const std::string& filename);
	virtual bool Output(const std::string& str);
	virtual bool OutputInteger(int value);
	virtual bool OutputDoubleNoLineFeed(double value);
	virtual bool OutputLong(long value);
	virtual bool OnStartHandling();
	virtual bool OnEndHandling();

private:
	std::list<std::string> m_Lines;
	std::string m_Filename;
};
