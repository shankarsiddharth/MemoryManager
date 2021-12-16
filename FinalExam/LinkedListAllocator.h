#pragma once
#include <cassert>

#include "MemoryBlock.h"

struct MemoryBlock;

class LinkedListAllocator
{
public:
	static LinkedListAllocator* Get();
	static LinkedListAllocator* Create(void* i_pMemoryAddress, size_t i_bytes, unsigned int i_numDescriptors);
	static LinkedListAllocator* Create(size_t i_bytes, unsigned int i_numDescriptors);
	void Destroy();

	void* Alloc(size_t i_size);	
	void* Alloc(size_t i_size, size_t i_align);
	bool Free(void* i_pMemory);
	void Collect();
	bool Contains(void* i_pMemory) const;
	bool IsAllocated(void* i_pMemory) const;
	size_t GetLargestFreeBlock() const;
	size_t GetTotalFreeMemory() const;
	void ShowFreeBlocks() const;	
	void ShowOutstandingAllocations() const;

private:
	static uintptr_t sBaseAddressOfLinkedListAllocator;
	static bool bIsVirtualMemoryAllocated;

	LinkedListAllocator* Initialize(void* i_pMemory, size_t i_bytes, unsigned int i_numDescriptors);
	static inline size_t GetSizeRequiredForANewBlock(size_t i_size, size_t i_align);
	MemoryBlock* FindFirstFit(size_t i_size, size_t i_align) const;
	void DisplayMemoryBlock(MemoryBlock* i_pMemoryBlock) const;
	void PrintDisplayHeading() const;

	bool canCoalesceWithNextMemoryBlock(MemoryBlock* i_pMemoryBlock);
	MemoryBlock* CoalesceWithNextBlock(MemoryBlock* i_pMemoryBlock);

	bool CheckAddressWithinHeap(uintptr_t i_AddressToCheck);

	uintptr_t pRoot = 0;
	MemoryBlock* pHead = nullptr;
	MemoryBlock* pTail = nullptr;
	size_t totalSizeOfLinkedListAllocator = 0;
};
