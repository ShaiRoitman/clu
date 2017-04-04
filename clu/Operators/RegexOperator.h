#include "InputFileOperator.h"
#include <boost/regex.hpp>

class RegexOperator : public clu::InputFileOperator
{
public:
	RegexOperator(std::string regex);
	virtual bool OnLineRead(std::string& line);
private:
	std::string delimitString;
	boost::regex m_regex;
};
