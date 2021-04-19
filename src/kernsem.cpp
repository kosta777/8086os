#include "kernsem.h"
#include <stdio.h>
SortedList *KernelSem::blocked = new SortedList();

int KernelSem::wait(Time time)
{
	val--;
	if(val < 0)
	{
		PCB::running->blocked = 1;
		PCB::running->blockingSem = this;
		PCB::running->semTimeExceeded = 0;
		blockedPCBs->insert(PCB::running);
		if(time > 0)
			blocked->insert(PCB::running, time);
		dispatchInterrupt();
	}
	return !PCB::running->semTimeExceeded;
}

void KernelSem::signal()
{
	val++;

	if(val <= 0)
	{
		PCB *pom = blockedPCBs->remove();
		pom->semTimeExceeded = 0;
		pom->blocked = 0;
		pom->blockingSem = NULL;
		blocked->remove(pom);
		Scheduler::put(pom);
	}
}

int KernelSem::value() const
{
	return val;
}

void KernelSem::UpdateTimer()
{
	if(!blocked->isEmpty())
		blocked->updateTime();
}
