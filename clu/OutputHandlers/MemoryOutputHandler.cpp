#pragma once

#include "MemoryOutputHandler.h"

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
	char Buffer[64] = { 0 };
#ifdef _WIN32
	sprintf_s(Buffer, 64, "%d\n", value);
#else
	sprintf(Buffer, "%d\n", value);
#endif
	m_Lines.push_back(std::string(Buffer));
	return true;
}
bool MemoryOutputHandler::OutputDoubleNoLineFeed(double value)
{
	char Buffer[64] = { 0 };
#ifdef _WIN32
	sprintf_s(Buffer, 64, "%.20g", value);
#else
	sprintf(Buffer, "%.20g", value);
#endif
	m_Lines.push_back(std::string(Buffer));
	return true;
}
bool MemoryOutputHandler::OutputLong(long value)
{
	char Buffer[64] = { 0 };
#ifdef _WIN32
	sprintf_s(Buffer, 64, "%ld\n", value);
#else
	sprintf(Buffer, "%ld\n", value);
#endif
	m_Lines.push_back(std::string(Buffer));
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
