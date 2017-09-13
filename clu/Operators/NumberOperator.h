#include "Utils/Utils.h"
#include "InputSource/InputFileOperator.h"
#include "CommandLineHandlers/PredicateCommandLineHandler.h"
#include <vector>
#include <map>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class NumberOperator : public InputFileOperator {
public:
	virtual bool OnNumberRead(double number) = 0;
	virtual bool OnLineRead(std::string& input); 
};
