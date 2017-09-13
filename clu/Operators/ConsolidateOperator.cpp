#include "InputFileOperator.h"
#include "CommandLineHandlers/SingleCommandLineHandler.h"
#include "NumberOperator.h"
#include <boost/algorithm/string/trim.hpp>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class ConsolidateAverage : public NumberOperator
{
public:
	ConsolidateAverage(int window_size) : m_size(window_size), m_data(0.0), m_count(0)
	{
	}

	virtual bool OnNumberRead(double number)
	{
		m_data = m_data + number;
		if (m_count == m_size) {
			m_OutputHandler->OutputDouble(m_data / m_count);
			m_count = 0;
			m_data = 0;
		}
		else {
			m_count++;
		}
		return true;
	}

	virtual void OnEnd()
	{
		m_data = 0.0;
		m_count = 0;
	}

protected:
	double m_data;
	int m_size;
	int m_count;
};

REGISTER_SINGLE_INTEGER("consolidate", ConsolidateAverage)->SetHelp("Consolidate - Average Out of groups of numbers");
