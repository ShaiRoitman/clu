#include "InputFileOperator.h"
#include "CommandLineHandlers/SingleCommandLineHandler.h"
#include <boost/regex.hpp>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class RegexHistogramOperator : public InputFileOperator {
public:
	RegexHistogramOperator(string regex) :
		m_regex(regex) {}
	virtual bool OnLineRead(string& input)
	{
		boost::match_results<string::const_iterator> matches;
		if (boost::regex_match(input, matches, m_regex))
		{
			string key = matches["Key"];
			if (key.length() == 0)
				key = matches[1];

			if (key.length() > 0)
			{
				string value = matches["Value"];
				double NumericValue = 1;
				if (value.length() > 0)
				{
					NumericValue = atof(value.c_str());
				}
				pair<double, int>& curValue = m_instanceCount[key];
				curValue.first += NumericValue;
				curValue.second++;
			}
		}

		return true;
	}

	virtual void OnEnd()
	{
		for (map <string, pair<double, int> >::iterator iter = m_instanceCount.begin();
		iter != m_instanceCount.end();
			++iter)
		{
			const string& value = (*iter).first;
			double& sum = (*iter).second.first;
			int& count = (*iter).second.second;
			m_OutputHandler->OutputLineFeed(StringValue(sum) + " : " + StringValue(count) + " : [" + value + "]");
		}

		m_instanceCount.clear();
	}
protected:
	map <string, pair<double, int> > m_instanceCount;
	boost::regex m_regex;
};

REGISTER_SINGLE_STRING("ehistogram", RegexHistogramOperator)->SetHelp("Uses regex to extract key and value - groups Key and Value");
