#include "MemoryOutputHandler.h"
#include <boost/lexical_cast.hpp>
#include <iomanip>

MemoryOutputHandler::MemoryOutputHandler()
{
}

bool MemoryOutputHandler::Output(const std::string& str)
{
	m_Lines.push_back(str);
	return true;
}
bool MemoryOutputHandler::OutputInteger(int value)
{
	const std::string& valueStr = boost::lexical_cast<std::string>(value);
	m_Lines.push_back(valueStr);
	return true;
}
bool MemoryOutputHandler::OutputDoubleNoLineFeed(double value)
{
	std::ostringstream ss;
	ss << std::fixed << std::setprecision(20);
	ss << value;
	const std::string& valueStr = ss.str();
	m_Lines.push_back(valueStr);
	return true;
}
bool MemoryOutputHandler::OutputLong(long value)
{
	const std::string& valueStr = boost::lexical_cast<std::string>(value);
	m_Lines.push_back(valueStr);
	return true;
}

bool MemoryOutputHandler::OnStartHandling()
{
	return true;
}

bool MemoryOutputHandler::OnEndHandling()
{
	return true;
}
