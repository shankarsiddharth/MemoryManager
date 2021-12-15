#include "MemorySystem.h"
#include "LinkedListAllocator.h"

bool InitializeMemorySystem(void * i_pHeapMemory, size_t i_sizeHeapMemory, unsigned int i_OptionalNumDescriptors)
{
	// create your HeapManager and FixedSizeAllocators
	LinkedListAllocator::Create(i_pHeapMemory, i_sizeHeapMemory, i_OptionalNumDescriptors);
	return true;
}

void Collect()
{
	// coalesce free blocks
	// you may or may not need to do this depending on how you've implemented your HeapManager
	LinkedListAllocator::Get()->Collect();
}

void DestroyMemorySystem()
{
	// Destroy your LinkedListAllocator and FixedSizeAllocators
	LinkedListAllocator::Get()->Destroy();
}

void DisplayFreeBlocks()
{
	LinkedListAllocator::Get()->ShowFreeBlocks();
}

void DisplayOutstandingBlocks()
{
	LinkedListAllocator::Get()->ShowOutstandingAllocations();
}

