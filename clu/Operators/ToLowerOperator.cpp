#include "Utils.h"
#include "InputFileOperator.h"
#include "CommandLineHandlers\PredicateCommandLineHandler.h"
#include <string>
#include <boost/tokenizer.hpp>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class ToLowerOperator : public InputFileOperator {
public:
	ToLowerOperator() {};
	virtual bool OnLineRead(string& line)
	{
		str_tolower(line);
		m_OutputHandler->OutputLineFeed(line);
		return true;
	}
private:
	void str_tolower(std::string &str) {
		std::transform(str.begin(), str.end(), str.begin(), (int(*)(int)) std::tolower);
	}

};

REGISTER_PREDICATE("lower", ToLowerOperator)->SetHelp("Change case to lower");
