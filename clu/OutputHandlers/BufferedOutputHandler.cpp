#include "BufferedOutputHandler.h"

BufferedOutputHandler::BufferedOutputHandler(const std::string& filename)
{
	m_Filename = filename;
}

bool BufferedOutputHandler::Output(const std::string& str)
{
	this->Output(str);
	return true;
}
bool BufferedOutputHandler::OutputInteger(int value)
{
	this->OutputInteger(value);
	return true;
}
bool BufferedOutputHandler::OutputDoubleNoLineFeed(double value)
{
	this->OutputDoubleNoLineFeed(value);
	return true;
}
bool BufferedOutputHandler::OutputLong(long value)
{
	this->OutputLong(value);
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
	for (std::list<std::string>::iterator iter = this->handler.m_Lines.begin(); iter != this->handler.m_Lines.end(); ++iter)
	{
		outFile << *iter;
	}

	outFile.close();

	return true;
}
