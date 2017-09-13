#include "InputFileOperator.h"
#include "CommandLineHandlers/SingleCommandLineHandler.h"
#include <boost/regex.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class UUIDOperator : public InputFileOperator
{
public:
	UUIDOperator(string regex)
	{
		m_regex = boost::regex(regex.c_str());
	}

	virtual bool OnLineRead(string& line)
	{
		string newValue = line;
		string prevValue = line;
		boost::uuids::basic_random_generator<boost::mt19937> gen;
		do
		{
			boost::uuids::uuid newID = gen();
			std::stringstream ss;
			ss << newID;
			string uuidstr;
			ss >> uuidstr;
			prevValue = newValue;
			newValue = regex_replace(prevValue, m_regex, uuidstr, boost::format_first_only);
		} while (prevValue != newValue);

		m_OutputHandler->OutputLineFeed(newValue);

		return true;
	}

	boost::regex m_regex;
};

REGISTER_SINGLE_STRING("uuid", UUIDOperator)->SetHelp("Replaces the value with uuid");

