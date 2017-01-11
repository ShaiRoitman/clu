#include "StandardInputSource.h"

StandardInputReader::StandardInputReader()
{
}

void StandardInputReader::GetLine(std::string& line)
{
	std::getline(std::cin, line);
}

bool StandardInputReader::IsEOF()
{
	return std::cin.eof();
}

