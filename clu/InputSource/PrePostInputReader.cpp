#include "PrePostInputSource.h"
#include "FileInputReader.h"
#include "StandardInputSource.h"

PreFileInputReader::PreFileInputReader(const std::string& filename)
{
	AddInputSource(new clu::FileInputReader(filename));
	AddInputSource(new StandardInputReader());
}


PostFileInputReader::PostFileInputReader(const std::string& filename)
{
	AddInputSource(new StandardInputReader());
	AddInputSource(new clu::FileInputReader(filename));
}

