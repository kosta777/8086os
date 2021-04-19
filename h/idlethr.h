#ifndef _idle_thr_h
#define _idle_thr_h

#include "thread.h"
#include <stdio.h>

class IdleThread: public Thread
{
public:

	IdleThread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice):Thread(stackSize, timeSlice)
	{

	}

	void run()
	{
		while(1)
		{
			for (int j = 0; j< 3000; ++j)
				for (int k = 0; k < 3000; ++k);
			//printf("idle\n");
		}

	}

};


#endif
