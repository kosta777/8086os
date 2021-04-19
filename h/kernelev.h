#ifndef _kernelev_h_
#define _kernelev_h_
#include "PCB.h"
#include "event.h"
#include "SCHEDULE.h"

class KernelEv
{
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();

	void wait();
	static unsigned cnt;
protected:
	friend class IVTEntry;
	void signal();
private:
	IVTNo ivtNo;
	unsigned ownerID;
	unsigned val;
	PCB *blockedPCB;
};
#endif

