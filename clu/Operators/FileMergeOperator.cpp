#include "NumberOperator.h"
#include "CommandLineHandlers\SingleCommandLineHandler.h"
#include <iostream>
#include <fstream>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class FileMergerOperator : public InputFileOperator {
public:
	FileMergerOperator(string filename) :
		m_filename(filename) {}

	virtual ~FileMergerOperator()
	{
		OnEnd();
	}
	virtual bool OnStart()
	{
		if (m_filename.length() > 0)
			m_ifs.open(m_filename.c_str());
		return true;
	}

	virtual void OnEnd() {}

	virtual bool OnLineRead(string& line)
	{
		string output_string;
		output_string = line;

		string file_line;
		if (!m_ifs.eof()) {
			getline(m_ifs, file_line);
			output_string = output_string + file_line;
		}

		m_OutputHandler->OutputLineFeed(output_string);
		return true;
	}

protected:
	string m_filename;
	ifstream m_ifs;
};

REGISTER_SINGLE_STRING("filemerge", FileMergerOperator)->SetHelp("FileMerge->line by line");
