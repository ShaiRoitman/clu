#include "InputFileOperator.h"
#include "CommandLineHandlers/CommandLineHandler.h"
#include <boost/tokenizer.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/program_options.hpp>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);
namespace po = boost::program_options;

class FilterOperator : public InputFileOperator 
{
public:
	FilterOperator(int argument_number) : m_argumentNumber(argument_number - 1)
	{
		if (argument_number == 0)
			m_fullline = true;
		else
			m_fullline = false;
	}

	virtual bool OnLineRead(string& input)
	{
		string argument;
		if (m_fullline)
			argument = input;
		else {
			vector<string> tokens;
			ExtractTokens(input, tokens, SpaceDelimiters);

			if (tokens.size() <= m_argumentNumber)
				return true;

			argument = tokens[m_argumentNumber];
		}

		if (shouldPrint(argument))
			m_OutputHandler->OutputLineFeed(input);
		return true;
	}

	virtual bool shouldPrint(string& data) = 0;
protected:
	size_t m_argumentNumber;
	bool m_fullline;
};

class NumericFilter : public FilterOperator
{
public:
	NumericFilter(int arg_num, double arg_value) : FilterOperator(arg_num), m_argValue(arg_value)
	{
	}

	virtual bool shouldPrint(string& data)
	{
		double cur_value = atof(data.c_str());
		return shouldPrint(cur_value);
	}

	virtual bool shouldPrint(double value) = 0;
protected:
	double m_argValue;
};

class GreaterThanFilter : public NumericFilter
{
public:
	GreaterThanFilter(int arg_num, double threshold) : NumericFilter(arg_num, threshold)
	{
	}
	virtual bool shouldPrint(double value)
	{
		return m_argValue < value;
	}
};

class LessThanFilter : public NumericFilter
{
public:
	LessThanFilter(int arg_num, double threshold) : NumericFilter(arg_num, threshold) {}
	virtual bool shouldPrint(double value)
	{
		return m_argValue > value;
	};
};

class ContainStringFilter : public FilterOperator
{
public:
	ContainStringFilter(int arg_num, string value, bool match) : FilterOperator(arg_num), m_match(match)
	{
		m_search_regex = boost::regex(value.c_str());
	}
	virtual bool shouldPrint(string& data)
	{
		bool ret_value = false;
		if (m_match)
			ret_value = boost::regex_match(data, m_search_regex);
		else
			ret_value = boost::regex_search(data, m_search_regex);

		return ret_value;
	}
protected:
	boost::regex m_search_regex;
	bool m_match;
};

class NotContainStringFilter : public ContainStringFilter
{
public:
	NotContainStringFilter(int arg_num, string value, bool match) : ContainStringFilter(arg_num, value, match)
	{
	}

	virtual bool shouldPrint(string& data)
	{
		return ContainStringFilter::shouldPrint(data);
	}
};

class Filter_CommandLine : public InputFileOperator_CommandLine
{
public:
	virtual bool AddProgramOptions(po::options_description& desc)
	{
		desc.add_options()
			("filter,f", po::value<int>(), "Filter Operator - Filter argument")
			("larger,l", po::value<double>(), "Filter Operator - Larger")
			("smaller,s", po::value<double>(), "Filter Operator - Smaller")
			("contains", po::value<string>(), "Filter Operator - Contains")
			("ncontains", po::value<string>(), "Filter Operator - Not Contains")
			("reg", po::bool_switch(), "Filter Operator - Contains / Not Contains regular expression, also in start stop operations");
		return true;
	}

	virtual InputFileOperator* GetOperator(const po::variables_map &vm)
	{
		InputFileOperator* listener = NULL;
		if (vm.count("filter")) {
			int filter_value = vm["filter"].as<int>();
			if (vm.count("larger")) {
				double larger_value = vm["larger"].as<double>();
				listener = new GreaterThanFilter(filter_value, larger_value);
			}
			if (vm.count("smaller")) {
				double smaller_value = vm["smaller"].as<double>();
				listener = new LessThanFilter(filter_value, smaller_value);
			}

			bool match = false;
			if (vm.count("reg") && vm["reg"].as<bool>())
				match = true;
			if (vm.count("contains")) {
				const string& contains_value = vm["contains"].as<string>();
				listener = new ContainStringFilter(filter_value, contains_value, match);
			}
			if (vm.count("ncontains")) {
				const string& ncontains_value = vm["ncontains"].as<string>();
				listener = new NotContainStringFilter(filter_value, ncontains_value, match);
			}
		}
		return listener;
	}
};

static auto nameRegisterVar = CommandLineHandler::GetInstance().AddCommandLineHandler(new Filter_CommandLine());
