#include "InputFileOperator.h"
#include "CommandLineHandlers/PredicateCommandLineHandler.h"

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class TrimOperator : public InputFileOperator
{
public:
	TrimOperator() {};
	virtual bool OnLineRead(string& input)
	{
		string spaceTrim = Trim(input, ' ');
		string tabTrim = Trim(spaceTrim, '\t');
		m_OutputHandler->OutputLineFeed(tabTrim);

		return true;
	}

	string Trim(string& input, char c)
	{
		size_t startOffset = input.find_first_not_of(c);
		size_t endOffset = input.find_last_not_of(c);
		if (startOffset == string::npos)
			return string("");
		if (endOffset == string::npos)
			return string("");
		return input.substr(startOffset, endOffset - startOffset + 1);
	}
};

REGISTER_PREDICATE("trim", TrimOperator)->SetHelp("Strips the input from both ends");
