#include <stdlib.h>

#include "hmap.h"

ThreadHashmap::ThreadHashmap()
{
	TABLE_SIZE=2;
	map = new HashEntry*[TABLE_SIZE];
	dummy = new HashEntry(-1, NULL);
	for(int i=0;i<TABLE_SIZE;i++)
		map[i] = NULL;
	size = 0;
}

void ThreadHashmap::Insert(int key, Thread* val)
{
	if(size == TABLE_SIZE)
	{
		int i;
		TABLE_SIZE *=2;
		HashEntry **newMap = new HashEntry*[TABLE_SIZE];
		for(i=0;i<TABLE_SIZE/2;i++)
			newMap[i] = map[i];
		for(i=TABLE_SIZE/2;i<TABLE_SIZE;i++)
			newMap[i] = NULL;
		HashEntry **pom = map;
		map = newMap;
		delete pom;
	}
	int hash = HashFunc(key);
	while(map[hash] != NULL && map[hash]->key != -1)
		hash = HashFunc(hash+1);
	map[hash] = new HashEntry(key, val);
	size++;
}

Thread* ThreadHashmap::Find(int key)
{
	int hash = HashFunc(key);
	while (map[hash] != NULL && map[hash]->key != key)
		hash = HashFunc(hash + 1);
	if (map[hash] == NULL)
		return NULL;
	return map[hash]->value;
}

void ThreadHashmap::Delete(int key)
{
	int hash = HashFunc(key);
	while (map[hash] != NULL && map[hash]->key != key)
		hash = HashFunc(hash + 1);
	if (map[hash] == NULL)
		return;
	HashEntry *temp = map[hash];
	map[hash] = dummy;
	delete temp;
}

ThreadHashmap::~ThreadHashmap()
{
	for (int i = 0; i < TABLE_SIZE; i++)
		if(map[i] != NULL)
			delete map[i];
	delete map;
}
