#pragma once

#include <stdio.h>
#include <vector>

class SimpleMemoryPool
{
public:
	SimpleMemoryPool(size_t t_memSize);
	~SimpleMemoryPool();

	static size_t numOfByteToBlock(size_t t_numByte);
	void* takeMemory(size_t t_numByte);
	void returnMemory(void* t_pointer, size_t t_numByte);
	bool isOwned(void* t_pointer);
	void printMemory() const;

private:
	unsigned char *m_memory;
	size_t m_allocatedMemory;
	bool *m_usedMemory;

	void allocateMemory(size_t t_numByte);
	int findAvailableLocation(size_t t_numBlock);
};

class PoolManager {

public:
	PoolManager(size_t t_memorySize);
	~PoolManager();

	void* takeMemory(size_t t_numByte);
	void returnMemory(void* t_pointer, size_t t_numByte);
	void printMemory() const;

private:
	std::vector<SimpleMemoryPool*> m_memoryPool;
	size_t m_memorySize;
};

