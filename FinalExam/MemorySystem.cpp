#include "MemorySystem.h"
#include "HeapManager.h"

bool InitializeMemorySystem(void * i_pHeapMemory, size_t i_sizeHeapMemory, unsigned int i_OptionalNumDescriptors)
{
	// create your HeapManager and FixedSizeAllocators
	HeapManager::Create(i_pHeapMemory, i_sizeHeapMemory, i_OptionalNumDescriptors);
	return true;
}

void Collect()
{
	// coalesce free blocks
	// you may or may not need to do this depending on how you've implemented your HeapManager
	HeapManager::Get()->Collect();
}

void DestroyMemorySystem()
{
	// Destroy your HeapManager and FixedSizeAllocators
	HeapManager::Get()->Destroy();
}

