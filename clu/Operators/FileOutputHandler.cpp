#include "OutputHandlers\FileOutputHandler.h"

FileOutputHandler::FileOutputHandler(const std::string filename)
{
	m_File = fopen(filename.c_str(), "wb");
	if (m_File == NULL)
	{
		printf("Could not open file [%s]\n", filename.c_str());
	}
}
FileOutputHandler::~FileOutputHandler()
{
	if (m_File)
		fclose(m_File);
}

bool FileOutputHandler::Output(const std::string& str)
{
	if (m_File)
		fprintf(m_File, "%s", str.c_str());
	return true;
}
bool FileOutputHandler::OutputInteger(int value)
{
	if (m_File)
		fprintf(m_File, "%d\n", value);
	return true;
}
bool FileOutputHandler::OutputDoubleNoLineFeed(double value)
{
	if (m_File)
		fprintf(m_File, "%.20g", value);
	return true;
}
bool FileOutputHandler::OutputLong(long value)
{
	if (m_File)
		fprintf(m_File, "%ld\n", value);
	return true;
}
