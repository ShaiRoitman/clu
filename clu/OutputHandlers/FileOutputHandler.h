#pragma once

#include "OutputHandler.h"

class FileOutputHandler : public OutputHandler
{
public:
	FileOutputHandler(const std::string filename);
	virtual ~FileOutputHandler();
	virtual bool Output(const std::string& str);
	virtual bool OutputInteger(int value);
	virtual bool OutputDoubleNoLineFeed(double value);
	virtual bool OutputLong(long value);
private:
	FILE* m_File;
};
