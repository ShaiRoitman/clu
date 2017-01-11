#include "OutputHandler.h"

OutputHandler::~OutputHandler()
{
}

bool OutputHandler::OnStartHandling() 
{ 
	return true; 
}

bool OutputHandler::OnEndHandling() 
{ 
	return true; 
}

bool OutputHandler::OutputDouble(double value)
{
	bool retValue = OutputDoubleNoLineFeed(value);
	Output("\n");
	return retValue;
}

bool OutputHandler::OutputLineFeed(const std::string& str)
{
	bool ret_value = Output(str);
	Output("\n");
	return ret_value;
}
