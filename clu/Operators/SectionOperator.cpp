#include "NumberOperator.h"
#include "CommandLineHandlers/SingleCommandLineHandler.h"
#include <boost/regex.hpp>

class SectionOperator : InputFileOperator
{
public:
	SectionOperator(const string& startSectionRegex, const string& endSectionRegex, int count)
	{
		m_StartRegex = boost::regex(startSectionRegex.c_str());
		m_EndRegex = boost::regex(endSectionRegex.c_str());
		m_Count = count;
		m_InSection = false;
	}

	virtual bool OnLineRead(string& line)
	{
		if (m_InSection) {

		}
		return true;
	}

	virtual void OnEnd()
	{
		if (m_InSection) {
			AddSection(m_CurrentSection);
		}

		for (list<string>::iterator iter = m_Sections.begin(); iter != m_Sections.end(); ++iter)
		{
			this->m_OutputHandler->Output(*iter);
		}
	}

private:
	virtual void AddSection(const string& sectionValue)
	{
		if (m_Sections.size() >= m_Count)
		{
			m_Sections.pop_front();
		}
		m_Sections.push_back(sectionValue);
	}

	size_t m_Count;
	boost::regex m_StartRegex;
	boost::regex m_EndRegex;
	list<string> m_Sections;
	string m_CurrentSection;
	bool m_InSection;
};
