#ifndef _semaphor_h_
#define _semaphor_h_
typedef unsigned int Time;
#include "kernsem.h"

class Semaphore
{
public:
	Semaphore (int init=1);
	virtual ~Semaphore ();
	virtual int wait (Time maxTimeToWait);
	virtual void signal();
	int val () const;
private:
	KernelSem* myImpl;
};
#endif
