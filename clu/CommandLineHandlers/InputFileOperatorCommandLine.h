#pragma once

#include "Utils.h"
#include "InputFileOperator.h"
#include <string>
#include <boost/program_options.hpp>

BEGIN_NAMESPACE(clu)

class InputFileOperator_CommandLine
{
public:
	virtual bool AddProgramOptions(boost::program_options::options_description& desc) = 0;
	virtual InputFileOperator* GetOperator(const boost::program_options::variables_map &vm) = 0;
};

END_NAMESPACE(clu)
