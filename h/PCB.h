#ifndef _pcb_h_
#define _pcb_h_
#include "thread.h"
#include "SCHEDULE.H"
#include <iostream.h>
#include "hmap.h"
#include <queue.h>
#include "queuesig.h"
#include "kernsem.h"

void signal0default();

class PCB
{
private:
	friend class Thread;
	friend class KernelSem;
	static int freeID;
	static ThreadHashmap *map;

	QueueSignal *signals;
	QueueSignal *signalsBlocked;
	unsigned signalMask; //pomeren za jedan zato sto se maskira i nula
	static unsigned signalMaskGlobal;
	unsigned signalBlockMask;
	static unsigned signalBlockMaskGlobal;
	void signal(SignalId signal);
	void registerHandler(SignalId signal, SignalHandler handler);
	SignalHandler getHandler(SignalId signal);
	void maskSignal(SignalId signal);
	static void maskSignalGlobally(SignalId signal);
	void unmaskSignal(SignalId signal);
	static void unmaskSignalGlobally(SignalId signal);
	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);
	static void pause();
	unsigned maskHit(SignalId, unsigned);
	void getSignalSettingsFromParent(PCB *parent);
public:
	unsigned sp;
	unsigned ss;
	unsigned bp;
	unsigned pomOFFS;
	unsigned pomSEG;
	unsigned finished;
	unsigned blocked;
	unsigned blockedEv;
	unsigned paused;
	unsigned semTimeExceeded;
	int kvant;
	int id;
	Thread *myThread;
	Thread *parentThread;
	KernelSem *blockingSem;
	static PCB *running;
	void handleSignals();
	unsigned handlingSignals;
	SignalHandler *signalHandlers;
	QueuePCB *blockedPCBs;
	unsigned *stackStart; //unistava se u thread destruktoru

	static void run()
	{
		running->myThread->run();
		running->finished = 1;
		dispatch();
	}

	PCB(Thread *tr = 0):id(freeID++)
	{
		map->Insert(id, myThread);
		myThread = tr;
		parentThread = PCB::running->myThread;
		getSignalSettingsFromParent(PCB::running);
		blocked = 0;
		blockedEv=0;
		paused = 0;
		blockedPCBs = new QueuePCB();

		signalHandlers = new SignalHandler[16];
		signalHandlers[0] = &signal0default;
		for(int i=1;i<16;i++)
			signalHandlers[i] = NULL;
		signals = new QueueSignal();
		signalsBlocked = new QueueSignal();
		signalMask = ~0;
		signalBlockMask = ~0;
		signalBlockMaskGlobal = ~0;
		handlingSignals = 0;
	}

	~PCB()
	{
		delete signals;
		delete signalsBlocked;
	}

	unsigned HasSignals()
	{
		return !signals->isEmpty() || !signalsBlocked->isEmpty();
	}
};
#endif
