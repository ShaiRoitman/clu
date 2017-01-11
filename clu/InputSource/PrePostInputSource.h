#include "MultipleInputSource.h"

class PreFileInputReader : public MultipleInputSource
{
public:
	PreFileInputReader(const std::string& filename);
};

class PostFileInputReader : public MultipleInputSource
{
public:
	PostFileInputReader(const std::string& filename);
};
