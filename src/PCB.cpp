#include "PCB.h"
#include "macros.h"
int PCB::freeID = 0;
PCB *PCB::running = 0;
ThreadHashmap *PCB::map = new ThreadHashmap();
unsigned PCB::signalMaskGlobal = ~0;
unsigned PCB::signalBlockMaskGlobal = ~0;

void signal0default()
{
	lockcpu
	PCB::running->handlingSignals = 0;
	PCB::running->finished = 1;
	dispatch();
	unlockcpu
}

void PCB::signal(SignalId signal)
{
	if(maskHit(signal, signalMask) && maskHit(signal, signalMaskGlobal))
	{
		if(!maskHit(signal, signalBlockMask) || !maskHit(signal, signalBlockMaskGlobal))
		{
			signalsBlocked->insert(signal);
			if(paused)
			{
				paused = 0;
				Scheduler::put(this);
			}
		}
		else
		{
			signals->insert(signal);
			if(paused)
			{
				paused = 0;
				Scheduler::put(this);
			}
		}
	}
}

void PCB::registerHandler(SignalId signal, SignalHandler handler)
{
	if(signal < 16)
		signalHandlers[signal] = handler;
}

SignalHandler PCB::getHandler(SignalId signal)
{
	return signalHandlers[signal];
}

void PCB::maskSignal(SignalId signal)
{
	signalMask = signalMask & ~(1 << signal);
}

void PCB::maskSignalGlobally(SignalId signal)
{
	signalMaskGlobal = signalMaskGlobal & ~(1 << signal);
}

void PCB::unmaskSignal(SignalId signal)
{
	signalMask = signalMask | (1 << signal);
}

void PCB::unmaskSignalGlobally(SignalId signal)
{
	signalMaskGlobal = signalMaskGlobal | (1 << signal);
}

void PCB::blockSignal(SignalId signal)
{
	signalBlockMask = signalBlockMask & ~(1 << signal);
}

void PCB::blockSignalGlobally(SignalId signal)
{
	signalBlockMaskGlobal = signalBlockMaskGlobal & ~(1 << signal);
}

void PCB::unblockSignal(SignalId signal)
{
	signalBlockMask = signalBlockMask | (1 << signal);
}

void PCB::unblockSignalGlobally(SignalId signal)
{
	signalBlockMaskGlobal = signalBlockMaskGlobal | (1 << signal);
	//for(int i=0;i< map->TABLE_SIZE; i++)
	//	if(map->map[i] != NULL)
	//		map->map[i]->value->unblockSignal(signal);
}

void PCB::pause()
{
	PCB::running->paused = 1;
	dispatch();
}

unsigned PCB::maskHit(SignalId signal, unsigned mask)
{
	return (1  << signal) & mask;
}

void PCB::getSignalSettingsFromParent(PCB *parent)
{
	signalMask = parent->signalMask;
	signalBlockMask = parent->signalBlockMask;
}

void PCB::handleSignals()
{
	handlingSignals =1;
	asm sti
	QueueSignal *newQueue = new QueueSignal();
	while(!signalsBlocked->isEmpty())
	{
		SignalId currentSignal = signalsBlocked->remove();
		if(!maskHit(currentSignal, signalBlockMask) || !maskHit(currentSignal, signalBlockMaskGlobal))
			newQueue->insert(currentSignal);
		else
			signals->insert(currentSignal);
	}
	delete signalsBlocked;
	signalsBlocked = newQueue;

	while(!signals->isEmpty())
	{
		SignalId currentSignal = signals->remove();
        if(!maskHit(currentSignal, signalMask))
        	continue;
		if(signalHandlers[currentSignal] != NULL)
			signalHandlers[currentSignal]();
	}
	asm cli
	handlingSignals = 0;
}
