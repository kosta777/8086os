#include <thread.h>
#include <dos.h>
#include <SCHEDULE.H>
#include <PCB.h>
#include <iostream.h>
#include <stdio.h>
#include "macros.h"

#include "hmap.h"
#include "queue.h"

Thread::Thread(StackSize stackSize, Time timeSlice)
{
	lockcpu
	if(stackSize > 64000)
		stackSize = 64000;
	myPCB = new PCB(this);
	unsigned* st1 = new unsigned[stackSize];
	st1[stackSize - 1] = 0x200;//setovan I fleg u pocetnom PSW-u za nit
	st1[stackSize - 2] = myPCB->pomSEG = FP_SEG(&PCB::run);
	st1[stackSize - 3] = myPCB->pomOFFS = FP_OFF(&PCB::run);
	myPCB->stackStart = st1;
	myPCB->sp = FP_OFF(st1 + stackSize - 12);
	myPCB->ss = FP_SEG(st1 + stackSize - 12);
	myPCB->bp = FP_OFF(st1 + stackSize - 12);
	myPCB->kvant = timeSlice;
	myPCB->finished = 0;
	unlockcpu
}

void Thread::start()
{
	Scheduler::put(myPCB);
}

void Thread::waitToComplete()
{
	lockcpu
	if(myPCB->finished == 0)
	{
		PCB::running->blocked = 1;
		myPCB->blockedPCBs->insert(PCB::running);
		dispatch();
	}
	unlockcpu
}

ID Thread::getId()
{
	return myPCB->id;
}

ID Thread::getRunningId()
{
	return PCB::running->id;
}

Thread * Thread::getThreadById(ID id)
{
	return PCB::map->Find(id);
}

Thread::~Thread()
{
	waitToComplete();
	lockcpu
	delete myPCB;
	unlockcpu
}

void Thread::signal(SignalId signal)
{
	lockcpu
	myPCB->signal(signal);
	unlockcpu
}

void Thread::registerHandler(SignalId signal, SignalHandler handler)
{
	lockcpu
	myPCB->registerHandler(signal, handler);
	unlockcpu
}

SignalHandler Thread::getHandler(SignalId signal)
{
	return myPCB->getHandler(signal);
}

void Thread::maskSignal(SignalId signal)
{
	lockcpu
	myPCB->maskSignal(signal);
	unlockcpu
}

void Thread::maskSignalGlobally(SignalId signal)
{
	lockcpu
	PCB::maskSignalGlobally(signal);
	unlockcpu
}

void Thread::unmaskSignal(SignalId signal)
{
	lockcpu
	myPCB->unmaskSignal(signal);
	unlockcpu
}

void Thread::unmaskSignalGlobally(SignalId signal)
{
	lockcpu
	PCB::unmaskSignalGlobally(signal);
	unlockcpu
}
void Thread::blockSignal(SignalId signal)
{
	lockcpu
	myPCB->blockSignal(signal);
	unlockcpu
}

void Thread::blockSignalGlobally(SignalId signal)
{
	lockcpu
	PCB::blockSignalGlobally(signal);
	unlockcpu
}

void Thread::unblockSignal(SignalId signal)
{
	lockcpu
	myPCB->unblockSignal(signal);
	unlockcpu
}

void Thread::unblockSignalGlobally(SignalId signal)
{
	lockcpu
	PCB::unblockSignalGlobally(signal);
	unlockcpu
}

void Thread::pause()
{
	lockcpu
	PCB::pause();
	unlockcpu
}
