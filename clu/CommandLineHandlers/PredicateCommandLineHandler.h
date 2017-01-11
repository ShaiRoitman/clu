#pragma once

#include "InputFileOperatorCommandLine.h"
#include "CommandLineHandler.h"

BEGIN_NAMESPACE(clu)

template <typename T>
class PredicateCommandLineHandler : public InputFileOperator_CommandLine
{
public:
	PredicateCommandLineHandler(const std::string& name) : m_optionName(name)
	{
	}

	virtual bool AddProgramOptions(boost::program_options::options_description& desc)
	{
		desc.add_options()
			(m_optionName.c_str(), boost::program_options::bool_switch(), m_help.c_str());
		return true;
	}

	virtual InputFileOperator* GetOperator(boost::program_options::variables_map &vm)
	{
		InputFileOperator* ret_value = NULL;
		if (vm.count(m_optionName.c_str()) && vm[m_optionName.c_str()].as<bool>())
			ret_value = new T();

		return ret_value;
	}

	PredicateCommandLineHandler* SetHelp(const std::string& help)
	{
		this->m_help = help;
		return this;
	}

protected:
	std::string m_optionName;
	std::string m_help;
};

#define REGISTER_PREDICATE(name, handler) \
  static clu::PredicateCommandLineHandler<handler>* nameRegisterVar = static_cast<clu::PredicateCommandLineHandler<handler>*>( \
    CommandLineHandler::GetInstance().AddCommandLineHandler( \
	  new PredicateCommandLineHandler<handler>(name)))



END_NAMESPACE(clu)