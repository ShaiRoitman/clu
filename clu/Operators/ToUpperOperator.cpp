#include "Utils.h"
#include "InputFileOperator.h"
#include "CommandLineHandlers/PredicateCommandLineHandler.h"
#include <string>
#include <boost/tokenizer.hpp>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class ToUpperOperator : public InputFileOperator {
public:
	ToUpperOperator() {};
	virtual bool OnLineRead(string& line)
	{
		str_toupper(line);
		m_OutputHandler->OutputLineFeed(line);
		return true;
	}
private:
	void str_toupper(std::string &str) {
		std::transform(str.begin(), str.end(), str.begin(), (int(*)(int)) std::toupper);
	}

};

REGISTER_PREDICATE("upper", ToUpperOperator)->SetHelp("Change case to upper");
