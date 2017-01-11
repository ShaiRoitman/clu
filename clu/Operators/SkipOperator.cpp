#include "Utils.h"
#include "InputFileOperator.h"
#include "CommandLineHandlers\SingleCommandLineHandler.h"
#include <boost/algorithm/string/trim.hpp>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class SkipFilter : public InputFileOperator {
public:
	SkipFilter(int startCounter) : m_start(startCounter), m_count(0) {}

	virtual bool OnLineRead(string& line)
	{
		if (m_count >= m_start)
			m_OutputHandler->OutputLineFeed(line);
		m_count++;
		return true;
	}

	virtual void OnEnd()
	{
		m_count = 0;
	}
protected:
	int m_start;
	int m_count;
};

REGISTER_SINGLE_INTEGER("skip", SkipFilter)->SetHelp("Skip        - Skips the first n lines");
