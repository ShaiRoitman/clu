#include "Utils.h"
#include "NumberOperator.h"
#include "CommandLineHandlers\PredicateCommandLineHandler.h"

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

bool NumberOperator::OnLineRead(std::string& input)
{
	vector<string> tokens;
	ExtractTokens(input, tokens, SpaceDelimiters);

	bool ret_value = true;
	if (tokens.size() > 0) {
		double value = atof(tokens[0].c_str());
		ret_value = OnNumberRead(value);
	}
	return ret_value;
}
