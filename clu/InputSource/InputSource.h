#pragma once

#include "InputFileOperator.h"

BEGIN_NAMESPACE(clu)

class InputSource
{
public:
	InputSource();
	virtual ~InputSource();
	virtual void SetListener(InputFileOperator* listener);
	virtual void GetLine(std::string& line) = 0;
	virtual bool IsEOF() = 0;
	virtual bool OnCloseSource();
	virtual void Read();
	
protected:
	InputFileOperator* m_Listener;
};

END_NAMESPACE(clu)