#include "event.h"
#include "kernelev.h"
#include "macros.h"

Event::Event(IVTNo ivtNo)
{
	lockcpu
	myImpl = new KernelEv(ivtNo);
	unlockcpu
}

Event::~Event()
{
	lockcpu
	delete myImpl;
	unlockcpu
}

void Event::wait()
{
	lockcpu
	myImpl->wait();
	unlockcpu
}

void Event::signal()
{

}
