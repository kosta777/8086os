#include "kernelev.h"
#include "ivtentry.h"
extern IVTEntry* IVTEntries[];
unsigned KernelEv::cnt = 0;
KernelEv::KernelEv(IVTNo ivtNo)
{
	val = 0;
	this->ivtNo = ivtNo;
	ownerID = PCB::running->id;
	blockedPCB=NULL;
	IVTEntry::IVTEntries[ivtNo]->reg(this);
}

void KernelEv::wait()
{
	lockcpu
	if(PCB::running->id == ownerID)
	{
		if(val == 0)
		{
			blockedPCB = PCB::running;
			PCB::running->blockedEv = 1;
			dispatch();
		}
		else
		{
			val = 0;
		}
	}
	unlockcpu
}

void KernelEv::signal()
{
	if(blockedPCB == NULL)
		val = 1;
	else
	{
		blockedPCB->blockedEv = 0;
		Scheduler::put(blockedPCB);
		blockedPCB=NULL;
		dispatchInterrupt();
	}
}

KernelEv::~KernelEv()
{

}
