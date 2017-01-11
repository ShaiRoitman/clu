#pragma once

#include "Utils.h"
#include "OutputHandler.h"
#include "StandardOutputHandler.h"

BEGIN_NAMESPACE(clu)

// A Line processor class
class InputFileOperator {
public:
	InputFileOperator()
	{
		m_OutputHandler = new StandardOutputHandler();
	}
	virtual ~InputFileOperator() {};
	// Is called before any processing is done
	// return false if not reading should occur
	virtual bool OnStart() { return true; };
	// Is called after each line is read
	// return true - to continue reading , false to stop 
	virtual bool OnLineRead(std::string& line) = 0;
	// Is called after all processing is done
	virtual void OnEnd() {};
	virtual int GetErrorCode() { return 0; }

public:
	OutputHandler* m_OutputHandler;
};

END_NAMESPACE(clu)
