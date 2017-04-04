#include "InputFileOperator.h"
#include "CommandLineHandlers\CommandLineHandler.h"

#include <boost/tokenizer.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/program_options.hpp>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class ArgOperator : public InputFileOperator
{
private:
	static const int maxNumberOfArguments = 500;
public:
	ArgOperator(string args, string delim, string print_delim)
	{
		m_delim = delim;
		m_print_delim = print_delim;
		typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
		std::string delimiter = GetEnvironmentOrDefault("CLU_ARG_DELIM", ",");
		boost::char_separator<char> sep(delimiter.c_str());
		boost::regex range_regex("([0-9]+)?-([0-9]+)?");

		tokenizer tok(args, sep);
		for (tokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg) {
			const string& arg_r = (*beg);
			boost::match_results<string::const_iterator> matches;
			if (boost::regex_match(arg_r, matches, range_regex)) {
				string start_range = matches[1];
				string end_range = matches[2];
				boost::algorithm::trim(start_range);
				boost::algorithm::trim(end_range);

				int start_range_numeric = 1;
				if (start_range.length() > 0)
					start_range_numeric = atoi(start_range.c_str());

				int end_range_numeric = maxNumberOfArguments;
				if (end_range.length() > 0)
					end_range_numeric = atoi(end_range.c_str()) + 1;

				for (int i = start_range_numeric; i < end_range_numeric; ++i) {
					m_args.push_back(i - 1);
				}
			}
			else {
				m_args.push_back(atoi(arg_r.c_str()) - 1);
			}
		}
	}

	virtual bool OnLineRead(string& input)
	{
		vector<string> tokens;
		ExtractTokens(input, tokens, m_delim.c_str());

		vector<int>::iterator iter;
		for (iter = m_args.begin(); iter != m_args.end(); iter++) {
			size_t arg_num = *iter;
			if (arg_num >= tokens.size())
				continue;
			m_OutputHandler->Output(tokens[arg_num]);
			m_OutputHandler->Output(m_print_delim);
		}
		m_OutputHandler->Output("\n");
		return true;
	}
public:
	vector<int> m_args;
	string m_delim;
	string m_print_delim;
};

class Arg_CommandLine : public InputFileOperator_CommandLine
{
	virtual bool AddProgramOptions(boost::program_options::options_description& desc)
	{
		desc.add_options()
			("arg,a", boost::program_options::value<string>(), "Arguments - Argument elements")
			("arg_delim,d", boost::program_options::value<string>(), "Arguments - Delimitors")
			("arg_print_delim", boost::program_options::value<string>(), "Arguments - Print Delimitor, default CLU_ARG_DELIM");
		return true;
	}

	virtual InputFileOperator* GetOperator(const boost::program_options::variables_map &vm)
	{
		InputFileOperator* listener = NULL;
		if (vm.count("arg")) {
			const string& arg = vm["arg"].as<string>();
			string delim = SpaceDelimiters;
			if (vm.count("arg_delim"))
				delim = vm["arg_delim"].as<string>();

			string print_delim = " ";
			if (vm.count("arg_print_delim"))
				print_delim = vm["arg_print_delim"].as<string>();

			listener = new ArgOperator(arg, delim, print_delim);
		}
		return listener;
	}
};

static auto nameRegisterVar = CommandLineHandler::GetInstance().AddCommandLineHandler(new Arg_CommandLine());
