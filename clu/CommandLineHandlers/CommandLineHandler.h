#pragma once

#include "Utils.h"
#include <memory>
#include "InputFileOperator.h"
#include "InputSource.h"
#include "InputFileOperatorCommandLine.h"
#include <list>

BEGIN_NAMESPACE(clu)

class CommandLineHandler
{
public:
	static CommandLineHandler& GetInstance();
	virtual ~CommandLineHandler();

	void Initialize(int argc, const char* argv[]);
	int ExecuteCommandLine();

	clu::InputFileOperator_CommandLine* AddCommandLineHandler(clu::InputFileOperator_CommandLine* handler);

	boost::program_options::options_description& GetOptionsDescriptions();
protected:
	int m_argc;
	const char** m_argv;
	std::list<InputFileOperator_CommandLine*> m_handlers;
	std::shared_ptr<boost::program_options::options_description> desc;
public:
	CommandLineHandler();
	static CommandLineHandler singleton;

	bool GetOperatorAndSource(InputFileOperator*& listener, InputSource*& source);
	bool addCommandLineOptions(boost::program_options::options_description& desc);
	bool parseCommandLineOptions(boost::program_options::variables_map& vm);
	clu::InputFileOperator* findCorrectHandler(boost::program_options::variables_map& vm);
};

END_NAMESPACE(clu)
