#include "InputFileOperator.h"
#include "CommandLineHandlers\SingleCommandLineHandler.h"

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class BucketOperator : public InputFileOperator
{
public:
	BucketOperator(string buckets)
	{
		std::vector<string> tokens;
		ExtractTokens(buckets, tokens, ",");
		for (std::vector<string>::iterator iter = tokens.begin();
		iter != tokens.end();
			++iter)
		{
			double currentValue = atof((*iter).c_str());
			m_Buckets.push_back(currentValue);
			m_Counts.push_back(0);
		}

		m_Buckets.push_back(DBL_MAX);
		m_Counts.push_back(0);

		std::sort(m_Buckets.begin(), m_Buckets.end());
	}

	virtual bool OnLineRead(string& line)
	{
		if (line.length() == 0)
			return true;

		std::vector<double>::iterator iter;
		std::vector<int>::iterator count_iter;
		double value = atof(line.c_str());
		for (iter = m_Buckets.begin(), count_iter = m_Counts.begin(); iter != m_Buckets.end(); ++iter, ++count_iter)
		{
			if (value < (*iter))
			{
				(*count_iter)++;
				break;
			}
		}

		return true;
	}

	virtual void OnEnd()
	{
		std::vector<double>::iterator last_Bucket = m_Buckets.end();
		last_Bucket--;
		for (std::vector<double>::iterator iter = m_Buckets.begin(); iter != last_Bucket; ++iter)
		{
			if (iter != m_Buckets.begin())
				m_OutputHandler->Output(",");
			m_OutputHandler->OutputDoubleNoLineFeed((*iter));
		}
		m_OutputHandler->OutputLineFeed("");

		for (std::vector<int>::iterator iter = m_Counts.begin(); iter != m_Counts.end(); ++iter)
		{
			if (iter != m_Counts.begin())
				m_OutputHandler->Output(",");
			m_OutputHandler->OutputDoubleNoLineFeed((*iter));
		}

		m_OutputHandler->OutputLineFeed("");

		m_Buckets.clear();
		m_Counts.clear();
	}

	std::vector<double> m_Buckets;
	std::vector<int> m_Counts;
};


REGISTER_SINGLE_STRING("bucket", BucketOperator)->SetHelp("Bucket values by there value, delimited by ,");
