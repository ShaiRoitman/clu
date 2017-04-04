#include "InputFileOperator.h"
#include "CommandLineHandlers\CommandLineHandler.h"
#include <boost/regex.hpp>
#include <boost/program_options.hpp>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class StartStopOperator : public InputFileOperator {
public:
	StartStopOperator(const string& start, const string& stop, bool match)
	{
		m_shouldMatch = match;
		if (start.length() == 0 && stop.length() > 0)
			m_shouldDisplay = true;
		else
			m_shouldDisplay = false;

		if (start.length() > 0)
			m_start_regex = boost::regex(start.c_str());

		if (stop.length() > 0)
			m_stop_regex = boost::regex(stop.c_str());
	}

	virtual bool OnLineRead(string& line)
	{
		if (m_shouldDisplay)
		{
			m_OutputHandler->OutputLineFeed(line);
			bool has_match = false;
			if (!m_stop_regex.empty())
			{
				if (m_shouldMatch)
					has_match = boost::regex_match(line, m_stop_regex);
				else
					has_match = boost::regex_search(line, m_stop_regex);
			}
			if (has_match)
				m_shouldDisplay = false;
		}
		else {
			bool has_match = false;
			if (!m_start_regex.empty())
			{
				if (m_shouldMatch)
					has_match = boost::regex_match(line, m_start_regex);
				else
					has_match = boost::regex_search(line, m_start_regex);
			}

			if (has_match) {
				m_OutputHandler->OutputLineFeed(line);
				m_shouldDisplay = true;
			}
		}
		return true;
	}

protected:
	boost::regex m_start_regex;
	boost::regex m_stop_regex;
	bool m_shouldDisplay;
	bool m_shouldMatch;
};

class StartStop_CommandLine : public InputFileOperator_CommandLine
{
	virtual bool AddProgramOptions(boost::program_options::options_description& desc)
	{
		desc.add_options()
			("start", boost::program_options::value<string>(), "Start Stop Operator - Start Regex")
			("stop", boost::program_options::value<string>(), "Start Stop Operator - Stop Regex");
		return true;
	}

	virtual InputFileOperator* GetOperator(const boost::program_options::variables_map &vm)
	{
		InputFileOperator* ret_value = NULL;
		if (vm.count("start") || vm.count("stop"))
		{
			string start;
			if (vm.count("start"))
				start = vm["start"].as<string>();
			string stop;
			if (vm.count("stop"))
				stop = vm["stop"].as<string>();

			bool should_match = false;
			if (vm.count("reg") && vm["reg"].as<bool>())
				should_match = true;
			ret_value = new StartStopOperator(start, stop, should_match);
		}
		return ret_value;
	}
};

static auto nameRegisterVar = CommandLineHandler::GetInstance().AddCommandLineHandler(new StartStop_CommandLine());
