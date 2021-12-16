#include "MemorySystem.h"
#include "HeapManager.h"
#include "FSAData.h"

bool InitializeMemorySystem(void* i_pHeapMemory, size_t i_sizeHeapMemory, unsigned int i_OptionalNumDescriptors)
{
	// create your HeapManager and FixedSizeAllocators
	FSAData FSAInfoData[] = { {96,400}, {32, 200}, {16, 100} };
	HeapManager::Create(i_pHeapMemory, i_sizeHeapMemory, i_OptionalNumDescriptors, FSAInfoData, sizeof(FSAInfoData)/sizeof(FSAData));
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
	// Destroy your LinkedListAllocator and FixedSizeAllocators
	HeapManager::Get()->Destroy();
}

void ShowFreeBlocks()
{
	HeapManager::Get()->ShowFreeBlocks();
}

void ShowOutstandingBlocks()
{
	HeapManager::Get()->ShowOutstandingAllocations();
}
