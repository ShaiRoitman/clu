#pragma once

#include "Utils.h"
#include "InputFileOperator.h"
#include "InputFileOperatorCommandLine.h"
#include <boost/program_options.hpp>

BEGIN_NAMESPACE(clu)

template <typename T>
class TwoStringCommandLineHandler : public InputFileOperator_CommandLine
{
public:
	TwoStringCommandLineHandler(std::string firstparam, std::string secondparam, std::string firsthelp, std::string secondhelp)
	{
		this->m_optionName1 = firstparam;
		this->m_optionName2 = secondparam;
		this->m_firsthelp = firsthelp;
		this->m_secondhelp = secondhelp;
	}

	virtual bool AddProgramOptions(boost::program_options::options_description& desc)
	{
		desc.add_options()
		  (m_optionName1.c_str(), boost::program_options::value<std::string>(), m_firsthelp.c_str());
		desc.add_options()
		  (m_optionName2.c_str(), boost::program_options::value<std::string>(), m_secondhelp.c_str());

		return true;
	}

	virtual clu::InputFileOperator* GetOperator(const boost::program_options::variables_map &vm)
	{
		InputFileOperator* ret_value = NULL;
		if (vm.count(m_optionName1.c_str()) != 0 &&
			vm.count(m_optionName2.c_str()) != 0)
		{
		  const std::string& arg1 = vm[m_optionName1.c_str()].template as<std::string>();
		  const std::string& arg2 = vm[m_optionName2.c_str()].template as<std::string>();
			ret_value = new T(arg1, arg2);
		}
		return ret_value;
	}

	std::string m_optionName1;
	std::string m_optionName2;
	std::string m_firsthelp;
	std::string m_secondhelp;
};

END_NAMESPACE(clu)
