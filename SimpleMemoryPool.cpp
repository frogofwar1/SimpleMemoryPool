#include "SimpleMemoryPool.hpp"

#include <iostream>
#include <stdlib.h>
#include <string.h>

const size_t DEFAULT_MEMORY_BLOCK_SIZE = 4;

SimpleMemoryPool::SimpleMemoryPool(size_t t_memSize) 
	: m_allocatedMemory(t_memSize)
{
	allocateMemory(t_memSize * DEFAULT_MEMORY_BLOCK_SIZE);
	m_usedMemory = new bool[t_memSize];
	memset(m_usedMemory, 0, t_memSize);
}


SimpleMemoryPool::~SimpleMemoryPool()
{
	free(m_memory);
	delete[] m_usedMemory;
}

size_t SimpleMemoryPool::numOfByteToBlock(size_t t_numByte) {

	return (t_numByte / DEFAULT_MEMORY_BLOCK_SIZE) +
		((t_numByte % DEFAULT_MEMORY_BLOCK_SIZE == 0) ? 0 : 1);
}

void* SimpleMemoryPool::takeMemory(size_t t_numByte) {

	size_t numBlock = numOfByteToBlock(t_numByte);
	int location = findAvailableLocation(numBlock);

	if (location < 0) {
		return nullptr; 
	}

	for (size_t i = 0; i < numBlock; i++) {
		m_usedMemory[location + i] = true;
	}

	return static_cast<void *> (m_memory + location * DEFAULT_MEMORY_BLOCK_SIZE);
}

void SimpleMemoryPool::returnMemory(void* t_pointer, size_t t_numByte) {

	size_t numBlock = numOfByteToBlock(t_numByte);
	size_t location = (static_cast<unsigned char *> (t_pointer) - m_memory) 
		/ DEFAULT_MEMORY_BLOCK_SIZE;

	for (size_t i = 0; i < numBlock; i++) {
		m_usedMemory[location + i] = false;
	}
}

bool SimpleMemoryPool::isOwned(void* t_pointer) {

	return t_pointer >= m_memory 
		&& t_pointer <= (m_memory + m_allocatedMemory * DEFAULT_MEMORY_BLOCK_SIZE);
}

void SimpleMemoryPool::printMemory() const {

	std::cout << "----------------------" << std::endl;
	std::cout << "--------Memory--------" << std::endl;
	std::cout << "----------------------" << std::endl;

	for (size_t i = 0; i < m_allocatedMemory; i++) {

		if (m_usedMemory[i]) {
			std::cout << "|M";
		}
		else {
			std::cout << "| ";
		}
	}

	std::cout << "|" << std::endl;
	std::cout << "----------------------" << std::endl;
}

void SimpleMemoryPool::allocateMemory(size_t t_numByte) {

	m_memory = static_cast<unsigned char *> (malloc(t_numByte));
}

int SimpleMemoryPool::findAvailableLocation(size_t t_numBlock) {

	for (unsigned int i = 0; i < m_allocatedMemory - (t_numBlock - 1); i++) {
		bool used = false;

		for (size_t block = 0; block < t_numBlock; block++) {
			used = used || m_usedMemory[i + block];
		}

		if (!used) {
			return i;
		}
	}
	return -1;
}

PoolManager::PoolManager(size_t t_memorySize)
	: m_memorySize(t_memorySize) 
{
	m_memoryPool.push_back(new SimpleMemoryPool(t_memorySize));
}

PoolManager::~PoolManager() 
{
	for (size_t i = 0; i < m_memoryPool.size(); i++) {
		delete m_memoryPool[i];
	}
};

void* PoolManager::takeMemory(size_t t_numByte) {
	for (size_t i = 0; i < m_memoryPool.size(); i++) {

		void *pointer = m_memoryPool[i]->takeMemory(t_numByte);

		if (pointer != nullptr) {
			return pointer; 
		}
	}

	SimpleMemoryPool *newPool = new SimpleMemoryPool(m_memorySize);
	m_memoryPool.push_back(newPool);
	return newPool->takeMemory(t_numByte);
}

void PoolManager::returnMemory(void* t_pointer, size_t t_numByte) {

	for (size_t i = 0; i < m_memoryPool.size(); i++) {

		if (m_memoryPool[i]->isOwned(t_pointer)) {
			m_memoryPool[i]->returnMemory(t_pointer, t_numByte);
		}
	}

}

void PoolManager::printMemory() const {

	std::cout << "-----Pool-Manager-----" << std::endl;

	for (size_t i = 0; i < m_memoryPool.size(); i++) {
		m_memoryPool[i]->printMemory();
	}

	std::cout << std::endl << std::endl << std::endl;
}