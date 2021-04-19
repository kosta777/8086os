#include "queuesig.h"

int QueueSignal::isEmpty()
{
    return count == 0;
}

void QueueSignal::insert(SignalId data){
    Node *newNode = new Node;
    newNode->data = data;
    newNode->next = NULL;
    if(isEmpty()){
        front = newNode;
    }
        else{
            back->next = newNode;
        }
        back = newNode;
        count++;
}

SignalId QueueSignal::remove()
{
	if(!isEmpty())
	{
		Node *temp = front;
		if(front == back)
		{
			front = NULL;
			back = NULL;
		}
		else
			front = front->next;
		count--;
		SignalId retVal = temp->data;
		delete temp;
		return retVal;
	}
	return NULL;
}

void QueueSignal::remove(SignalId target)
{
	Node *curr = front;
	Node *last = NULL;
	while(curr != NULL &&  target != curr->data)
	{
		last = curr;
		curr = curr->next;
	}
	if(curr != NULL)
	{
		if(last == NULL)
		{
			front = curr->next;
			if(front == NULL)
				back = NULL;
			delete curr;
			return;
		}
		last->next = curr->next;
		if(curr == back)
			back = last;
		delete curr;
	}
}
