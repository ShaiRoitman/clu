#include "InputFileOperator.h"
#include "CommandLineHandlers/SingleCommandLineHandler.h"
#include "NumberOperator.h"
#include <boost/algorithm/string/trim.hpp>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class SmoothOperator : public NumberOperator
{
public:
	SmoothOperator(int window_size) : m_size(window_size), m_data(NULL), m_curpos(0), m_currsize(0)
	{
		m_data = new double[window_size];
		for (int i = 0; i < window_size; i++) {
			m_data[i] = 0;
		}
	}

	virtual ~SmoothOperator()
	{
		delete m_data;
	}

	virtual bool OnNumberRead(double number)
	{
		m_data[m_curpos] = number;
		m_curpos++;
		m_curpos = m_curpos % m_size;
		m_currsize++;

		int cur_size = m_currsize;
		if (cur_size > m_size)
			cur_size = m_size;

		double total = 0;
		for (int i = 0; i < cur_size; i++) {
			total = total + m_data[i];
		}
		m_OutputHandler->OutputDouble(total / cur_size);
		return true;
	}

	virtual void OnEnd()
	{
		for (int i = 0; i < m_size; i++)
			m_data[i] = 0;
		m_curpos = 0;
	}

protected:
	int m_size;
	double* m_data;
	int m_currsize;
	int m_curpos;
};

REGISTER_SINGLE_INTEGER("smooth", SmoothOperator)->SetHelp("Smooth Size - Running Average");
