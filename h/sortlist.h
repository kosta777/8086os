#ifndef _sorted_list_h
#define _sorted_list_h

#include "PCB.h"
#include "stdlib.h"

class SortedList 
{
public:
	SortedList();
	friend class KernelSem;
	void insert(PCB *pcb, int value);
	void updateTime();
	void remove(PCB *pcb);
	unsigned isEmpty() const;

private:
	struct Elem 
	{
		PCB *pcb;
		int value;
		Elem *next;
		Elem(PCB *pcb, int val)
		{
			this->value = val;
			this->pcb = pcb;
			next = NULL;
		}
	};
	Elem *first;

};

#endif
