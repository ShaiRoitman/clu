#include "AggregateOperator.h"
#include "CommandLineHandlers\PredicateCommandLineHandler.h"
#include <stdlib.h>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class ErrorCodeHandler : public InputFileOperator {
public:
	ErrorCodeHandler() : m_error_code(0) {};
	virtual bool OnLineRead(string& line)
	{
		m_error_code = atoi(line.c_str());
		return false;
	}
	virtual int GetErrorCode() { return m_error_code; }
protected:
	int m_error_code;
};

REGISTER_PREDICATE("err_code", ErrorCodeHandler)->SetHelp("Return Value as Error Code");
