#include "SimpleMemoryPool.hpp"

#include <iostream>

#define _CRTDBG_MAP_ALLOC  

struct Monster {
	int Hp;
	int Atk;
};

struct Item {
	size_t id;
};

int main() {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::cout << "Create Memory Pool Manager " << std::endl << std::endl;
	PoolManager poolManager(10);
	poolManager.printMemory();

	std::cout << "Allocate Memory for Monster " << sizeof(Monster) << " Bytes" << std::endl << std::endl;
	Monster *monster = static_cast<Monster*> (poolManager.takeMemory(sizeof(Monster)));
	poolManager.printMemory();

	std::cout << "Allocate Memory for Item " << sizeof(Item) << " Bytes" << std::endl << std::endl;
	Item *item = static_cast<Item*> (poolManager.takeMemory(sizeof(Item)));
	poolManager.printMemory();

	std::cout << "Allocate Memory for Monster2 " << sizeof(Monster) << " Bytes" << std::endl << std::endl;
	Monster *monster2 = static_cast<Monster*> (poolManager.takeMemory(sizeof(Monster)));
	poolManager.printMemory();

	std::cout << "Return Memory for Monster " << sizeof(Monster) << " Bytes" << std::endl << std::endl;
	poolManager.returnMemory(static_cast<void *> (monster), sizeof(Monster));
	poolManager.printMemory();

	std::cout << "Return Memory for Monster2 " << sizeof(Monster) << " Bytes" << std::endl << std::endl;
	poolManager.returnMemory(static_cast<void *> (monster2), sizeof(Monster));
	poolManager.printMemory();

	std::cout << "Allocate Memory for Item2 " << sizeof(Item) << " Bytes" << std::endl << std::endl;
	Item *item2 = static_cast<Item*> (poolManager.takeMemory(sizeof(Item)));
	poolManager.printMemory();

	std::cout << "Return Memory for Item " << sizeof(Item) << " Bytes" << std::endl << std::endl;
	poolManager.returnMemory(static_cast<void *> (item), sizeof(Item));
	poolManager.printMemory();

	getchar();
}