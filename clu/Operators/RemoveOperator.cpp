#include "InputFileOperator.h"
#include "CommandLineHandlers\SingleCommandLineHandler.h"
#include <boost/algorithm/string/trim.hpp>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class RemoveOperator : public InputFileOperator
{
public:
	RemoveOperator(string remove_file) : m_removefile(remove_file) {};
	virtual bool OnStart()
	{
		ifstream ifs(m_removefile.c_str());
		string line;
		while (getline(ifs, line)) {
			boost::algorithm::trim(line);
			m_strings.insert(line);
		}
		return true;
	}

	virtual bool OnLineRead(string& line)
	{
		if (m_strings.find(line) == m_strings.end()) {
			m_OutputHandler->OutputLineFeed(line);
		}
		return true;
	}
protected:
	string m_removefile;
	set< string > m_strings;
};

REGISTER_SINGLE_STRING("remove", RemoveOperator)->SetHelp("Remove From input the file lines");
