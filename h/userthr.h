#ifndef USERTHR_H_
#define USERTHR_H_
#include "thread.h"
extern userMain(int, char**);
class userthr : public Thread
{
public:
	int param1;
	char **param2;
	userthr(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice):Thread(stackSize, timeSlice)
	{}

	void run()
	{
		userMain(param1, param2);
	}

};

#endif /* USERTHR_H_ */
