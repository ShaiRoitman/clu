#include "InputFileOperator.h"
#include "CommandLineHandlers/SingleCommandLineHandler.h"

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class ReplaceQuotesOperator : public InputFileOperator 
{
public:
	ReplaceQuotesOperator(string replace) :
		m_replaceString(replace) {}

	virtual bool OnLineRead(string& input)
	{
		string outputString = ReplaceAllValues(input, string("\""), this->m_replaceString);
		m_OutputHandler->OutputLineFeed(outputString);
		return true;
	}

protected:
	string m_replaceString;
};

REGISTER_SINGLE_STRING("replaceQuotes", ReplaceQuotesOperator)->SetHelp("Replace quotes by the string");
