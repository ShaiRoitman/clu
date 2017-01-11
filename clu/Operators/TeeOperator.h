#include "InputFileOperator.h"
#include "OutputHandler.h"

class TeeOperator : public clu::InputFileOperator 
{
public:
	TeeOperator(clu::InputFileOperator* listener, const std::string& filename);
	virtual bool OnStart();
	virtual bool OnLineRead(std::string& line);
	virtual void OnEnd();
private:
	clu::InputFileOperator* m_Listener;
	OutputHandler* m_InnerOutputHandler;
};

