#include "ivtentry.h"
#include <dos.h>
#include "kernelev.h"
#include "macros.h"

pInterrupt IVTEntry::oldIVT[256];
IVTEntry* IVTEntry::IVTEntries[256];
unsigned IVTEntry::ivtInit = 0;

IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt newIR)
{
	lockcpu
	if(!ivtInit)
	{
		ivtInit = 1;
		inicIVT();
	}
	IVTEntries[ivtNo] = this;
	this->ivtNo = ivtNo;
	setvect(ivtNo, newIR);
	unlockcpu
}

void IVTEntry::signal()
{
	if(event != NULL)
		event->signal();
}

void IVTEntry::reg(KernelEv *ev)
{
	lockcpu
	this->event = ev;
	unlockcpu
}

