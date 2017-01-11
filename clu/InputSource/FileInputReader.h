#pragma once

#include "InputSource.h"

BEGIN_NAMESPACE(clu)

class FileInputReader : public InputSource {
public:
	FileInputReader(std::string filename);
	void GetLine(std::string& line);
	virtual bool IsEOF();
	virtual bool OnCloseSource();
protected:
	std::string m_filename;
	std::ifstream m_ifs;
};

END_NAMESPACE(clu)