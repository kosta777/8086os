#ifndef _queue_h_
#define _queue_h_
#include <stdlib.h>

class QueuePCB
{
public:
	friend class PCB;
	QueuePCB(): front(NULL), back(NULL), count(0) {}
	void insert(PCB* data);
	PCB* remove();
	void remove(PCB* targetPCB);
	int isEmpty();
private:
       struct Node
	   {
           PCB* data;
           Node *next;
       };

       Node *front;
       Node *back;
       int count;
};

#endif
