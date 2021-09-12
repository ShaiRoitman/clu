#pragma once

#include "Utils.h"
#include "InputFileOperatorCommandLine.h"
#include "CommandLineHandlers/CommandLineHandler.h"

#include <string>

BEGIN_NAMESPACE(clu)

template <typename T, typename Arg>
class SingleCommandLineHandler : public InputFileOperator_CommandLine
{
public:
	SingleCommandLineHandler(std::string name) : m_optionName(name)
	{
	}

	virtual bool AddProgramOptions(boost::program_options::options_description& desc)
	{
		desc.add_options()
			(m_optionName.c_str(), boost::program_options::value<Arg>(), m_help.c_str());
		return true;
	}

	virtual InputFileOperator* GetOperator(const boost::program_options::variables_map &vm)
	{
		InputFileOperator* ret_value = NULL;
		if (vm.count(m_optionName.c_str())) {
			const Arg& arg = vm[m_optionName.c_str()].template as<Arg>();
			ret_value = new T(arg);
		}
		return ret_value;
	}

	SingleCommandLineHandler<T, Arg>* SetHelp(const std::string& help)
	{
		this->m_help = help;
		return this;
	}

protected:
	std::string m_help;
	std::string m_optionName;
};

template <typename T>
class SingleStringCommandLineHandler : public SingleCommandLineHandler<T, std::string>
{
public:
	SingleStringCommandLineHandler(std::string name) : SingleCommandLineHandler<T, std::string>(name)
	{
	}
};

template <typename T>
class SingleIntegerCommandLineHandler : public SingleCommandLineHandler<T, int>
{
public:
	SingleIntegerCommandLineHandler(std::string name) : SingleCommandLineHandler<T, int>(name)
	{
	}
};

template <typename T>
class SingleLongCommandLineHandler : public SingleCommandLineHandler<T, long>
{
public:
	SingleLongCommandLineHandler(std::string name) : SingleCommandLineHandler<T, long>(name)
	{
	}
};

template <typename T>
class SingleDoubleCommandLineHandler : public SingleCommandLineHandler<T, double>
{
public:
	SingleDoubleCommandLineHandler(std::string name) : SingleCommandLineHandler<T, double>(name)
	{
	}
};

#define REGISTER_SINGLE_STRING(name, handler) \
  static clu::SingleCommandLineHandler<handler, std::string>* nameRegisterVar = static_cast<clu::SingleCommandLineHandler<handler, std::string>*>( \
    CommandLineHandler::GetInstance().AddCommandLineHandler( \
	  new clu::SingleCommandLineHandler<handler, std::string>(name)))

#define REGISTER_SINGLE_INTEGER(name, handler) \
  static clu::SingleCommandLineHandler<handler, int>* nameRegisterVar = static_cast<clu::SingleCommandLineHandler<handler, int>*>( \
    CommandLineHandler::GetInstance().AddCommandLineHandler( \
	  new clu::SingleCommandLineHandler<handler, int>(name)))

#define REGISTER_SINGLE_LONG(name, handler) \
  static clu::SingleCommandLineHandler<handler, long>* nameRegisterVar = static_cast<clu::SingleCommandLineHandler<handler, long>*>( \
    CommandLineHandler::GetInstance().AddCommandLineHandler( \
	  new clu::SingleCommandLineHandler<handler, long>(name)))

#define REGISTER_SINGLE_DOUBLE(name, handler) \
  static clu::SingleCommandLineHandler<handler, double>* nameRegisterVar = static_cast<clu::SingleCommandLineHandler<handler, double>*>( \
    CommandLineHandler::GetInstance().AddCommandLineHandler( \
	  new clu::SingleCommandLineHandler<handler, double>(name)))

END_NAMESPACE(clu)
