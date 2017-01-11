#include "MultipleInputSource.h"

USING_NAMESPACE(clu)
USING_NAMESPACE(std)

MultipleInputSource::MultipleInputSource()
{
}

MultipleInputSource::~MultipleInputSource()
{
	list <InputSource*>::iterator iter;
	for (iter = m_Sources.begin(); iter != m_Sources.end(); ++iter)
	{
		delete *iter;
	}
}

void MultipleInputSource::AddInputSource(InputSource* input)
{
	m_Sources.push_back(input);
}

void MultipleInputSource::Read()
{
	m_CurrentInput = m_Sources.begin();
	InputSource::Read();
}

void MultipleInputSource::GetLine(string& line)
{
	InputSource* currentSource = *m_CurrentInput;
	currentSource->GetLine(line);
}

bool MultipleInputSource::IsEOF()
{
	if (m_CurrentInput == m_Sources.end())
		return true;
	InputSource* currentSource = *m_CurrentInput;
	bool retValue = currentSource->IsEOF();

	if (retValue == true) {
		m_CurrentInput++;
		if (m_CurrentInput != m_Sources.end())
			retValue = false;
		retValue = currentSource->IsEOF();
	}

	return retValue;
}
