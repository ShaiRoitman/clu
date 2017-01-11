#include "FullFileOperator.h"
#include "CommandLineHandlers\SingleCommandLineHandler.h"

#include <vector>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class PercentileOperator : public FullFileOperator
{
public:
	PercentileOperator(double percentile) : m_Percentile(percentile) {};
	virtual void OnEnd()
	{
		vector<double> values;
		values.reserve(m_data.size());
		values.resize(m_data.size());
		int currentCount = 0;
		for (size_t i = 0; i < m_data.size(); i++)
		{
			try
			{
				double currentValue = boost::lexical_cast<double>(m_data[i]);
				values[currentCount] = currentValue;
				currentCount++;
			}
			catch (boost::bad_lexical_cast c)
			{
			}
		}

		sort(values.begin(), values.begin() + currentCount);
		int percentileLocation = max(0, min(round(currentCount * m_Percentile) - 1, currentCount));
		m_OutputHandler->OutputDouble(values[percentileLocation]);

		FullFileOperator::OnEnd();
	}

private:
	int round(double r)
	{
		return (r > 0.0) ? (int)floor(r + 0.5) : (int)ceil(r - 0.5);
	}
	double m_Percentile;
};

REGISTER_SINGLE_DOUBLE("percentile", PercentileOperator)->SetHelp("Extracts the percentile");

