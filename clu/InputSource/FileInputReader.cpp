#include "FileInputReader.h"

#include "Utils.h"
#include "OutputHandler.h"

#include <iostream>
#include <fstream>

BEGIN_NAMESPACE(clu)

FileInputReader::FileInputReader(std::string filename) : m_filename(filename)
{
	m_ifs.open(m_filename.c_str());
}
void FileInputReader::GetLine(std::string& line)
{
	getline(m_ifs, line);
}

bool FileInputReader::IsEOF()
{
	if (m_ifs.is_open())
		return m_ifs.eof();
	else
		return true;
}

bool FileInputReader::OnCloseSource()
{
	m_ifs.close();
	return true;
}

END_NAMESPACE(clu)