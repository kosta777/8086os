#ifndef _kern_sem_h
#define _kern_sem_h
#include "queue.h"
#include "sortlist.h"
class KernelSem
{
public:
	friend class SortedList;
	KernelSem(int initValue = 1):val(initValue), lck(0)
	{
		blockedPCBs = new QueuePCB();
	}
	int wait(Time time);
	void signal();
	int value() const;
	static void UpdateTimer();
	static SortedList *blocked;
private:
	int val;
	int lck;
	QueuePCB *blockedPCBs;
};

#endif
