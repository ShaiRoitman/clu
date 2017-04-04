#include "InputFileOperator.h"
#include <functional>

clu::InputFileOperator* CreateParallelOperator(
	int countSize, 
	int numberOfThreads, 
	int queueSize, 
	std::function<clu::InputFileOperator*()> operatorFactory);
