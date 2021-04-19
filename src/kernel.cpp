#ifndef _kernel_cpp_
#define _kernel_cpp_

#include <thread.h>
#include <iostream.h>
#include <dos.h>
#include "SCHEDULE.H"
#include <PCB.h>
#include "semaphor.h"
#include "kernsem.h"
#include "kernel.h"
#include "ivtentry.h"
#include "event.h"
#include "user.h"
#include "userthr.h"



volatile unsigned tsp;
volatile unsigned tss;
volatile unsigned tbp;
extern void tick();

volatile int counter = 20;
volatile int context_switch_req = 0;
volatile unsigned unlimited_time = 0;
volatile pInterrupt oldTimer;

void interrupt timer(...)
{
	unsigned csFlag = 0;
	if (!context_switch_req) { if(!unlimited_time)counter--; KernelSem::UpdateTimer(); }
	if (counter <= 0 || context_switch_req)
	{
		if(!PCB::running->handlingSignals)
		{
			csFlag = 1;
#ifndef BCC_BLOCK_IGNORE
			asm {
				// cuva sp
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}
#endif

			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;
			if(!PCB::running->finished && !PCB::running->blocked && !PCB::running->blockedEv && !PCB::running->paused && !idle->active)
				Scheduler::put(PCB::running);

			if(PCB::running->finished)
			{
				while(!PCB::running->blockedPCBs->isEmpty())
				{
					PCB *blockedPCB = PCB::running->blockedPCBs->remove();
					blockedPCB->blocked = 0;
					Scheduler::put(blockedPCB);
				}
				if(PCB::running->signalHandlers[2] != NULL)
					PCB::running->signalHandlers[2]();
				if(PCB::running->parentThread != NULL)
					PCB::running->parentThread->signal(1);
				delete PCB::running->stackStart;
			}

			PCB *newPCB = Scheduler::get();
			if(!newPCB)
			{
				PCB::running = idle->idlePCB;
				idle->active = 1;
			}
			else
			{
				PCB::running = newPCB;
				idle->active = 0;
			}
			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;
			counter = PCB::running->kvant;
			if(counter == 0)
			{
				counter = 1;
				unlimited_time = 1;
			}
			else
				unlimited_time = 0;

#ifndef BCC_BLOCK_IGNORE
			asm {
				mov sp, tsp   // restore sp
				mov ss, tss
				mov bp, tbp
			}
#endif
			if(PCB::running->HasSignals())
				PCB::running->handleSignals();
		}
	}
	if(!context_switch_req)
	{
		tick();
		oldTimer();
	}

	if(csFlag)
		context_switch_req = 0;
}

void dispatch(){ // sinhrona promena konteksta
	asm cli;
	context_switch_req = 1;
	timer();
	asm sti;
}


void dispatchInterrupt(){ // sinhrona promena konteksta
	context_switch_req = 1;
	timer();
}

void inic()
{
	oldTimer = getvect(8);
	setvect(8, &timer);
	PCB::running = new PCB();
	PCB::running->finished = 0;
	PCB::running->kvant = 20;

	idle->idleThr->start();
	idle->idlePCB = Scheduler::get();

}

// vraca staru prekidnu rutinu
void restore()
{
	lockcpu
	restoreIVT();
	setvect(8, oldTimer);
	unlockcpu
}

extern userMain(int, char**);
int main(int a, char** b)
{
	inic();

	Thread *user = new userthr();
	((userthr*)user)->param1 = a;
	((userthr*)user)->param2 = b;
	user->start();
	user->waitToComplete();

/*
	for(int i=0;i<20;i++)
	{
		for(int j=0;j<30000;j++)
			for(int z=0;z<30000;z++);
	}
*/

	printf("Happy End main-a\n");
	fflush(stdout);
	restore();

	return 0;
}

#endif
