#include "InputSource.h"

BEGIN_NAMESPACE(clu)

InputSource::InputSource() : m_Listener(nullptr)
{
}

InputSource::~InputSource()
{
}

void InputSource::SetListener(InputFileOperator* listener)
{
	m_Listener = listener;
}

bool InputSource::OnCloseSource() { return true; }

void InputSource::Read()
{
	if (m_Listener != NULL)
	{
		m_Listener->m_OutputHandler->OnStartHandling();
		bool shouldContinue = m_Listener->OnStart();
		while (!IsEOF() && shouldContinue) {
			std::string input;
			GetLine(input);
			if (!IsEOF() || input.size() > 0)
			{
				shouldContinue = m_Listener->OnLineRead(input);
			}
		}
		OnCloseSource();
		m_Listener->OnEnd();
		m_Listener->m_OutputHandler->OnEndHandling();
	}
}

END_NAMESPACE(clu)