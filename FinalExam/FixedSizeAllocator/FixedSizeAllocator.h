#pragma once
#include <cstdint>

#include "BitArray.h"
#include "FSAData.h"

class FixedSizeAllocator
{
public:
	static FixedSizeAllocator* Create(void* i_pBaseAddressOfAvailableMemory, size_t i_sizeOfAvailableMemoryMemory, size_t i_sizeOfBlock, size_t i_numberOfBlocks);
	void Destroy();

	FixedSizeAllocator* Initialize(uintptr_t i_rootAddress, size_t i_totalSize,
		size_t i_sizeOfBlock, size_t i_numberOfBlocks, uintptr_t i_pBaseAddressOfAvailableMemory, size_t i_remainingAvailableSize, uintptr_t i_alignedBaseAddressOfFixedSizeMemoryBlocks);

	void* Alloc(size_t i_size);
	bool Free(void* i_pMemory);
	bool Contains(void* i_pMemory);

	static size_t GetRequiredSizeForBlocks(size_t i_sizeOfBlock, size_t i_numberOfBlocks);

private:
	uintptr_t pRoot = 0;
	size_t totalSize = 0;
	FSAData FSAInfo = {0,0};
	BitArray* pFSABlockData = nullptr;
	uintptr_t pBaseAddressOfFixedSizeMemoryBlocks = 0;
};

