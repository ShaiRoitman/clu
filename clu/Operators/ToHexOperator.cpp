#include "InputFileOperator.h"
#include "CommandLineHandlers/PredicateCommandLineHandler.h"
#include "NumberOperator.h"

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class ToHexOperator : public NumberOperator
{
public:
	ToHexOperator()
	{
	}

	virtual bool OnNumberRead(double number)
	{
		char buffer[64] = { 0 };
		long integerNumber = (long)number;
		sprintf(buffer, "%x", integerNumber);
		m_OutputHandler->OutputLineFeed(std::string(buffer));
		return true;
	}
};

REGISTER_PREDICATE("tohex", ToHexOperator)->SetHelp("To Hex convertor");
