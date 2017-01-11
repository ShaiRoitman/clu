#include "InputSource.h"

class StandardInputReader : public clu::InputSource {
public:
	StandardInputReader();

	virtual void GetLine(std::string& line);
	virtual bool IsEOF();
};
