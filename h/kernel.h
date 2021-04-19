#ifndef _kernel_h_
#define _kernel_h_
#include "idlethr.h"
#include "PCB.h"
#include "SCHEDULE.h"

struct Idle{
	IdleThread *idleThr;
	PCB *idlePCB;
	unsigned active;
	Idle()
	{
		idleThr = new IdleThread(defaultStackSize, 2);
		idlePCB = NULL;
		active = 0;
	}
};
Idle *idle = new Idle();

#endif
