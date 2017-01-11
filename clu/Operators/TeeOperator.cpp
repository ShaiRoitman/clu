#include "TeeOperator.h"
#include "OutputHandlers\FileOutputHandler.h"

USING_NAMESPACE(clu)
USING_NAMESPACE(std)

TeeOperator::TeeOperator(InputFileOperator* listener, const string& filename) : m_Listener(listener)
{
	m_InnerOutputHandler = new FileOutputHandler(filename);
	m_Listener->m_OutputHandler = m_InnerOutputHandler;
}

bool TeeOperator::OnStart()
{
	return m_Listener->OnStart();
}

bool TeeOperator::OnLineRead(string& line)
{
	m_OutputHandler->OutputLineFeed(line);
	return m_Listener->OnLineRead(line);
}

void TeeOperator::OnEnd()
{
	m_Listener->OnEnd();
	m_Listener->m_OutputHandler = m_OutputHandler;
	delete m_InnerOutputHandler;
	m_InnerOutputHandler = NULL;
}

