#pragma once
#include <cassert>

#include "MemoryBlock.h"

struct MemoryBlock;

class HeapManager
{
public:
	static HeapManager* Get();
	static HeapManager* Create(void* i_pHeapMemory, size_t i_bytes, unsigned int i_numDescriptors);
	static HeapManager* Create(size_t i_bytes, unsigned int i_numDescriptors);
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

	static inline size_t AlignSize(size_t i_size, size_t i_align = sDefaultAlignment) {
		return (i_size + i_align - 1) & ~(i_align - 1);
	}

	static inline size_t AlignPadding(size_t i_size, size_t i_align = sDefaultAlignment) {
		return AlignSize(i_size, i_align) - i_size;
	}

	static inline bool IsPowerOfTwo(size_t i_align)
	{
		return (i_align & (i_align - 1)) == 0;
	}

	static inline uintptr_t AlignDown(uintptr_t i_value, size_t i_align)
	{
		assert(i_align);
		assert(IsPowerOfTwo(i_align));
		return i_value & ~(i_align - 1);
	}

	static inline uintptr_t AlignUp(uintptr_t i_value, size_t i_align)
	{
		assert(i_align);
		assert(IsPowerOfTwo(i_align));
		return (i_value + (i_align - 1)) & ~(i_align - 1);
	}

	static const size_t sDefaultAlignment;

private:
	static uintptr_t sBaseAddressOfHeapManager;

	HeapManager* Initialize(void* i_pMemory, size_t i_bytes, unsigned int i_numDescriptors);
	//HeapManager* Initialize(size_t i_Bytes);
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
	size_t totalHeapSize = 0;
};
