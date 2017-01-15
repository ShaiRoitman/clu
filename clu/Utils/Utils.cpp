#include "Utils.h"
#include <boost/tokenizer.hpp>
#include <cstdlib>
#include "Globals.h"

USING_NAMESPACE(std);

const char* g_version = "5.2.0";
const char* g_app_name = "Command Line Utility";
const char* g_developer_name = "Shai Roitman";


std::string StringValue(double i)
{
	char buffer[100] = { 0 };
#ifdef _WIN32
	sprintf_s(buffer, 100, "%g", i);
#else
	sprintf(buffer, "%g", i);
#endif
	return string(buffer);
}

void ExtractTokens(string input, vector<string>& ret_value, const char* delim)
{
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sep(delim);
	tokenizer tok(input, sep);
	for (tokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg) {
		ret_value.push_back(*beg);
	}
}

string GetEnvironmentOrDefault(string envName, const char* defaultValue)
{
	string retValue;

	char *envValue = getenv(envName.c_str());
	if (envValue == NULL)
	{
		retValue = defaultValue;
	}
	else
	{
		retValue = envValue;
	}

	return retValue;
}

int GetEnvironmentOrDefaultInteger(string envName, int defaultValue)
{
	string retValue = GetEnvironmentOrDefault(envName, StringValue(defaultValue).c_str());

	return atoi(retValue.c_str());
}


string ReplaceAllValues(string sourceLine, string search, string replace)
{
	do
	{
		size_t offset = sourceLine.find(search);
		if (offset == string::npos)
			break;
		sourceLine = sourceLine.replace(offset, search.length(), replace);
	} while (true);

	return sourceLine;
}
