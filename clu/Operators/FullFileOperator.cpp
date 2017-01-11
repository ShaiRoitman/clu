#include "FullFileOperator.h"

bool FullFileOperator::OnLineRead(string& line)
{
	m_data.push_back(line);
	return true;
}

void FullFileOperator::PrintData()
{
	for (vector<string>::iterator iter = m_data.begin(); iter != m_data.end(); ++iter)
	{
		string& cur_line = *iter;
		m_OutputHandler->OutputLineFeed(cur_line);
	}
}

void FullFileOperator::PrintReverseData()
{
	for (vector<string>::reverse_iterator iter = m_data.rbegin(); iter != m_data.rend(); ++iter)
	{
		string& cur_line = *iter;
		m_OutputHandler->OutputLineFeed(cur_line);
	}
}

void FullFileOperator::OnEnd()
{
	m_data.clear();
}
