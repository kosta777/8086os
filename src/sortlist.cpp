#include "sortlist.h"
#include "SCHEDULE.h"
#include <stdio.h>

SortedList::SortedList()
{
	first = NULL;
}

void SortedList::insert(PCB *pcb, int value)
{
	Elem *newElem = new Elem(pcb, value);
	if (first == NULL)
		first = newElem;
	else
	{
		Elem *curr = first;
		Elem *last = NULL;
		while (curr != NULL && value > curr->value)
		{
			last = curr;
			curr = curr->next;
		}
		if (last == NULL)
		{
			newElem->next = first;
			first = newElem;
		}
		else
		{
			Elem * temp = last->next;
			last->next = newElem;
			newElem->next = temp;
		}
	}
}

void SortedList::updateTime()
{
	Elem *curr = first;
	while (curr != NULL)
	{
		Elem *next = curr->next;
		curr->value -= 1;
		if (curr->value <= 0)
		{
			PCB *myPCB = curr->pcb;
			myPCB->blockingSem->blockedPCBs->remove(myPCB);
			myPCB->semTimeExceeded = 1;
			myPCB->blockingSem->val++;
			myPCB->blocked = 0;
			Scheduler::put(myPCB);
			first = curr->next;
			delete curr;
		}
		curr = next;
	}
}

void SortedList::remove(PCB *pcb)
{
		Elem *curr = first;
		Elem *last = NULL;
		while (curr != NULL && curr->pcb->id != pcb->id)
		{
			last = curr;
			curr = curr->next;
		}
		if (curr != NULL)
		{
			if (last == NULL)
			{
				first = first->next;
				return;
			}
			last->next = curr->next;
			delete curr;
		}
}

unsigned SortedList::isEmpty() const
{
	return first == NULL;
}
