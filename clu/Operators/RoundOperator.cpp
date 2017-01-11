#include "InputFileOperator.h"
#include "CommandLineHandlers\SingleCommandLineHandler.h"
#include "NumberOperator.h"
#include <boost/algorithm/string/trim.hpp>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class RoundOperator : public NumberOperator
{
public:
	RoundOperator(int value) : m_value(value)
	{
	}

	virtual bool OnNumberRead(double number)
	{
		long roundedNumber = (number > 0.0) ? (long)(number) : (long)(number)-1;

		long reminder = roundedNumber % m_value;
		roundedNumber -= reminder;
		if (abs(reminder) >= (m_value / 2))
		{
			if (number > 0)
				roundedNumber += m_value;
			else
				roundedNumber -= m_value;
		}

		m_OutputHandler->OutputInteger(roundedNumber);
		return true;
	}

protected:
	long m_value;
};

REGISTER_SINGLE_INTEGER("round", RoundOperator)->SetHelp("Round       - Round up to the nearest");
