#include "semaphor.h"
#include "macros.h"

Semaphore::Semaphore (int init)
{
	lockcpu
	myImpl = new KernelSem(init);
	unlockcpu
}

Semaphore::~Semaphore ()
{
	lockcpu
	delete myImpl;
	unlockcpu
}

int Semaphore::wait(Time maxTimeToWait)
{
	lockcpu
	int rez = myImpl->wait(maxTimeToWait);
	unlockcpu
	return rez;
}

void Semaphore::signal()
{
	lockcpu
	myImpl->signal();
	unlockcpu
}

int Semaphore::val () const
{
	return myImpl->value();
}
