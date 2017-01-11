#include "StandardOutputHandler.h"

bool StandardOutputHandler::Output(const std::string& str)
{
	printf("%s", str.c_str());
	return true;
}
bool StandardOutputHandler::OutputInteger(int value)
{
	printf("%d\n", value);
	return true;
}
bool StandardOutputHandler::OutputDoubleNoLineFeed(double value)
{
	printf("%.20g", value);
	return true;
}
bool StandardOutputHandler::OutputLong(long value)
{
	printf("%ld\n", value);
	return true;
}
