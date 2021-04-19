#ifndef _ivt_entry_h
#define _ivt_entry_h
#include <dos.h>
#include "kernelev.h"
#include "macros.h"
#include <stdio.h>
#include <iostream.h>

typedef void interrupt (*pInterrupt)(...);

class IVTEntry;

class IVTEntry
{
public:
	IVTEntry(IVTNo ivtNo, pInterrupt newIR);
	void signal();
	void reg(KernelEv *ev);

	static pInterrupt oldIVT[256];
	static IVTEntry* IVTEntries[256];
	static unsigned ivtInit;
protected:
	IVTNo ivtNo;
	KernelEv *event;
};

static void inicIVT()
{
	lockcpu
	for(int i=0;i<256;i++)
		IVTEntry::oldIVT[i] = getvect(i);
	unlockcpu
}

static void restoreIVT()
{
	if(IVTEntry::ivtInit)
	{
		lockcpu
		for(int i=0;i<256;i++)
			setvect(i, IVTEntry::oldIVT[i]);
		unlockcpu
	}
}

#endif

#define PREPAREENTRY(num, callOld) \
void interrupt IVTFunc##num(...); \
IVTEntry *IVTEntry##num = new IVTEntry(num, &IVTFunc##num); \
void interrupt IVTFunc##num(...)\
{\
	IVTEntry##num->signal();\
	if(callOld)\
		IVTEntry::oldIVT[num]();\
}


