#include "InputFileOperator.h"
#include "CommandLineHandlers\CommandLineHandler.h"

#include <boost/algorithm/string/trim.hpp>
#include <boost/program_options.hpp>
#include "NumberOperator.h"

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class LinearOperator : public NumberOperator
{
public:
	LinearOperator(double div, double mul, double add) : m_mul(mul), m_add(add)
	{
		if (div != 0)
			m_mul = m_mul / div;
	}
	virtual bool OnNumberRead(double number)
	{
		m_OutputHandler->OutputDouble(number * m_mul + m_add);
		return true;
	}
private:
	double m_mul;
	double m_add;
};


class Linear_CommandLine : public InputFileOperator_CommandLine
{
	virtual bool AddProgramOptions(boost::program_options::options_description& desc)
	{
		desc.add_options()
			("add", boost::program_options::value<double>(), "Linear Addition")
			("mul", boost::program_options::value<double>(), "Linear Multiplication")
			("div", boost::program_options::value<double>(), "Linear Division");
		return true;
	}

	virtual InputFileOperator* GetOperator(const boost::program_options::variables_map &vm)
	{
		InputFileOperator* ret_value = NULL;
		if (vm.count("add") || vm.count("mul") || vm.count("div")) {
			double add_value = 0;
			if (vm.count("add"))
				add_value = vm["add"].as<double>();

			double mul_value = 1;
			if (vm.count("mul"))
				mul_value = vm["mul"].as<double>();

			double div_value = 1;
			if (vm.count("div"))
				div_value = vm["div"].as<double>();

			ret_value = new LinearOperator(div_value, mul_value, add_value);
		}
		return ret_value;
	}
};

static auto nameRegisterVar = CommandLineHandler::GetInstance().AddCommandLineHandler(new Linear_CommandLine());
