#pragma once

#include "BufferedOutputHandler.h"

BufferedOutputHandler::BufferedOutputHandler(const std::string& filename)
{
	m_Filename = filename;
}

bool BufferedOutputHandler::Output(const std::string& str)
{
	m_Lines.push_back(str);
	return true;
}
bool BufferedOutputHandler::OutputInteger(int value)
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
bool BufferedOutputHandler::OutputDoubleNoLineFeed(double value)
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
bool BufferedOutputHandler::OutputLong(long value)
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

bool BufferedOutputHandler::OnStartHandling()
{
	return true;
}

bool BufferedOutputHandler::OnEndHandling()
{
	std::ofstream outFile;

	outFile.open(m_Filename.c_str());
	for (std::list<std::string>::iterator iter = m_Lines.begin(); iter != m_Lines.end(); ++iter)
	{
		outFile << *iter;
	}

	outFile.close();

	return true;
}
