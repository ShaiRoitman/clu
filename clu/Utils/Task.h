#ifndef TASK_HEADER
#define TASK_HEADER

class Task
{
public:
	virtual ~Task() {}
	virtual void operator()() = 0;
};


#endif