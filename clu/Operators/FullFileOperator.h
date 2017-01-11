#include "InputFileOperator.h"

#include <vector>
#include <string>
USING_NAMESPACE(std);
USING_NAMESPACE(clu);


class FullFileOperator : public InputFileOperator
{
public:
	virtual bool OnLineRead(std::string& line);
	virtual void PrintData();
	virtual void PrintReverseData();
	virtual void OnEnd();
protected:
	std::vector<std::string> m_data;
};
