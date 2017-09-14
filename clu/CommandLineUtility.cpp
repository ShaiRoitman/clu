#include "Initialization.h"
#include "CommandLineHandlers/CommandLineHandler.h"

USING_NAMESPACE(clu)

int main(int argc, const char* argv[])
{
	InitCluEnvironment();
	CommandLineHandler& handler = CommandLineHandler::GetInstance();
	handler.Initialize(argc, argv);
	int ret_value = handler.ExecuteCommandLine();
	return ret_value;
}
