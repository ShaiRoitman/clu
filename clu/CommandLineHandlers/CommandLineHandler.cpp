#include "CommandLineHandler.h"
#include <memory>
#include "InputFileOperator.h"
#include "OutputHandlers\FileOutputHandler.h"
#include "OutputHandlers\BufferedOutputHandler.h"
#include "FileInputReader.h"
#include "Globals.h"
#include "Operators\TeeOperator.h"
#include "PrePostInputSource.h"
#include "StandardInputSource.h"
#include "Operators\ParallelOperator.h"
#include <boost/regex.hpp>

BEGIN_NAMESPACE(clu)
USING_NAMESPACE(std)

CommandLineHandler CommandLineHandler::singleton;

CommandLineHandler& CommandLineHandler::GetInstance()
{
	return singleton;
}

CommandLineHandler::CommandLineHandler()
{
	this->desc = std::make_shared<boost::program_options::options_description>(*new boost::program_options::options_description("Allowed options for " + string(g_app_name)));
}

CommandLineHandler::~CommandLineHandler()
{
}

void CommandLineHandler::Initialize(int argc, const char* argv[])
{
	this->m_argc = argc;
	this->m_argv = argv;
}

clu::InputFileOperator_CommandLine* CommandLineHandler::AddCommandLineHandler(clu::InputFileOperator_CommandLine* handler)
{
	m_handlers.push_back(handler);
	return handler;
}

bool CommandLineHandler::addCommandLineOptions(boost::program_options::options_description& desc)
{
	try
	{
		std::list<InputFileOperator_CommandLine*>::iterator iter;
		for (iter = m_handlers.begin();
		iter != m_handlers.end();
			++iter)
		{
			InputFileOperator_CommandLine* current_handler = *iter;
			current_handler->AddProgramOptions(desc);
		}
	}
	catch (exception& e) {
		cerr << "error: " << e.what() << "\n";
		return false;
	}
	catch (...) {
		cerr << "Exception of unknown type!\n";
		return false;
	}
	return true;
}

bool CommandLineHandler::parseCommandLineOptions(boost::program_options::variables_map& vm)
{
	try {
		boost::program_options::store(boost::program_options::parse_command_line(m_argc, m_argv, *this->desc), vm);
		boost::program_options::notify(vm);
	}
	catch (exception& e) {
		cerr << "error: " << e.what() << "\n";
		return false;
	}
	catch (...) {
		cerr << "Exception of unknown type!\n";
		return false;
	}
	return true;
}

boost::program_options::options_description& CommandLineHandler::GetOptionsDescriptions()
{
	return *this->desc;
}

bool CommandLineHandler::GetOperatorAndSource(InputFileOperator*& listener, InputSource*& source)
{
	listener = NULL;
	source = NULL;
	boost::program_options::variables_map vm;
	bool success_code = addCommandLineOptions(*desc);
	if (success_code == false)
		return NULL;

	desc->add_options()
		("output,o", boost::program_options::value<string>(), "Output result to a file , prints to standard output the input");
	desc->add_options()
		("input,i", boost::program_options::value<string>(), "Uses the file as the input stream");
	desc->add_options()
		("preinput", boost::program_options::value<string>(), "Uses the file as the input stream");
	desc->add_options()
		("postinput", boost::program_options::value<string>(), "Uses the file as the input stream");

	desc->add_options()
		("inplace", boost::program_options::value<string>(), "Works on a file inplace");

	desc->add_options()
		("parallel", boost::program_options::value<string>(), "Parallel, ChunkSize, NumberOfThreads, QueueSize");

	success_code = parseCommandLineOptions(vm);
	if (success_code == false)
		return NULL;

	if (vm.count("parallel"))
	{
		boost::regex pOptions ("(?<ChunkSize>[0-9]+),(?<Threads>[0-9]+),(?<QueueSize>[0-9]+)");
		const string& pParams = vm["parallel"].as<string>();
		int chunkSize = 50000;
		int numberOfThreads = 4;
		int queueSize = 3;

		boost::match_results<string::const_iterator> matches;
		if (boost::regex_match(pParams, matches, pOptions))
		{
			chunkSize = atoi(matches["ChunkSize"].str().c_str());
			numberOfThreads = atoi(matches["Threads"].str().c_str());
			queueSize = atoi(matches["QueueSize"].str().c_str());
		}

		boost::program_options::variables_map* passedVM = new boost::program_options::variables_map(vm);
		listener = CreateParallelOperator(
			chunkSize, 
			numberOfThreads, 
			queueSize, 
			[=]() 
		{
			boost::program_options::variables_map* passVMCopy = passedVM;
			return this->findCorrectHandler(*passVMCopy);
		});
	}
	else
	{
		listener = findCorrectHandler(vm);
	}


	if (vm.count("output") && listener != NULL) {
		const string& output_filename = vm["output"].as<string>();
		InputFileOperator* tee_operator = new TeeOperator(listener, output_filename);
		listener = tee_operator;
	}

	if (vm.count("input"))
	{
		const string& input_filename = vm["input"].as<string>();
		source = new FileInputReader(input_filename);
	}
	else if (vm.count("preinput"))
	{
		const string& input_filename = vm["preinput"].as<string>();
		source = new PreFileInputReader(input_filename);
	}
	else if (vm.count("postinput"))
	{
		const string& input_filename = vm["postinput"].as<string>();
		source = new PostFileInputReader(input_filename);
	}
	else if (vm.count("inplace"))
	{
		const string& inplace_filename = vm["inplace"].as<string>();
		source = new FileInputReader(inplace_filename);
		listener->m_OutputHandler = new BufferedOutputHandler(inplace_filename);
	}
	else
	{
		source = new StandardInputReader();
	}

	return true;
}

int CommandLineHandler::ExecuteCommandLine()
{
	int ret_value = 0;
	InputFileOperator* listener = nullptr;
	InputSource* source = nullptr;
	try
	{
		if (GetOperatorAndSource(listener, source) != true)
		{
			cout << "Invalid Operators";
		}
	}
	catch (const std::exception& e) {
		cout << "Got Exception " << e.what();
	}

	if (listener != NULL && source != NULL)
	{
		unique_ptr<InputFileOperator> listener_guard(listener);
		unique_ptr<InputSource> input_source(source);
		source->SetListener(listener);
		source->Read();
		ret_value = listener->GetErrorCode();
	}
	else
	{
		ret_value = 1;
	}

	return ret_value;
}

InputFileOperator* CommandLineHandler::findCorrectHandler(boost::program_options::variables_map& vm)
{
	InputFileOperator* ret_value = NULL;
	std::list<InputFileOperator_CommandLine*>::iterator iter;
	for (iter = m_handlers.begin();
		 iter != m_handlers.end() && ret_value == NULL;
		 ++iter)
	{
		InputFileOperator_CommandLine* current_handler = *iter;
		ret_value = current_handler->GetOperator(vm);
	}

	return ret_value;
}

END_NAMESPACE(clu)
