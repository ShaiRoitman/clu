#include "NumberOperator.h"
#include "CommandLineHandlers/SingleCommandLineHandler.h"

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class ModuleOperator : public NumberOperator
{
public:
	ModuleOperator(int mod) : m_mod(mod)
	{
	}
	virtual bool OnNumberRead(double number)
	{
		int res = (int)number % m_mod;
		m_OutputHandler->OutputInteger(res);
		return true;
	}
private:
	int m_mod;
};

REGISTER_SINGLE_INTEGER("mod", ModuleOperator)->SetHelp("Module math operator");
