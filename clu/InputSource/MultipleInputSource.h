#include "InputSource.h"
#include <list>

class MultipleInputSource : public clu::InputSource
{
public:
	MultipleInputSource();
	virtual ~MultipleInputSource();
	virtual void AddInputSource(clu::InputSource* input);
	virtual void Read();
	virtual void GetLine(std::string& line);
	virtual bool IsEOF();
protected:
	std::list<clu::InputSource *> m_Sources;
	std::list<clu::InputSource *>::iterator m_CurrentInput;

};
