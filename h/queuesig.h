#ifndef _queue_sig_h_
#define _queue_h_sig_
#include <stdlib.h>
#include "thread.h"

class QueueSignal
{
public:
	QueueSignal(): front(NULL), back(NULL), count(0) {}
	void insert(SignalId data);
	SignalId remove();
	void remove(SignalId target);
	int isEmpty();
private:
       struct Node
	   {
           SignalId data;
           Node *next;
       };

       Node *front;
       Node *back;
       int count;
};

#endif
