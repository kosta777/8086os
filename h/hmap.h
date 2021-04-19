#ifndef _thread_hashmap_h_
#define _thread_hashmap_h_
#include "thread.h"

class ThreadHashmap
{
public:
	friend class PCB;
	ThreadHashmap();
	//ThreadHashmap(const ThreadHashmap&) = delete;
	Thread *Find(int key);
	void Insert(int key, Thread* val);
	void Delete(int key);
	~ThreadHashmap();

private:
	int TABLE_SIZE;
	struct HashEntry
	{
		public:
			int key;
			Thread *value;
			HashEntry(int key, Thread *value)
			{
				this->key = key;
				this->value = value;
			}
	};
	HashEntry **map;
	HashEntry *dummy;
	int size;
	int HashFunc(int key)
	{
		return key % TABLE_SIZE;
	}
};

#endif
