#include "queue.h"
#include <stdio.h>

int QueuePCB::isEmpty()
{
    return count == 0;
}

void QueuePCB::insert(PCB* data){
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

PCB* QueuePCB::remove()
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
		PCB* retVal = temp->data;
		delete temp;
		return retVal;
	}
	return NULL;
}

void QueuePCB::remove(PCB *targetPCB)
{
	Node *curr = front;
	Node *last = NULL;
	while(curr != NULL &&  targetPCB != curr->data)
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
