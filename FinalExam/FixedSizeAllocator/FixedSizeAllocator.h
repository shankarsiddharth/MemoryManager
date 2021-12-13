#pragma once
#include <cstdint>

#include "BitArray.h"
#include "FSAData.h"

class FixedSizeAllocator
{
public:
	static FixedSizeAllocator* Create(void* i_pBaseMemory, size_t i_sizeOFMemory, size_t i_sizeOfBlock, size_t i_numberOfBlocks);
	void Destroy();

	void* Alloc(size_t i_size);
	bool Free(void* i_pMemory);
	bool Contains(void* i_pMemory);

private:
	uintptr_t pRoot = 0;
	FSAData* pFSAData = nullptr;
	BitArray* pFSABlockData = nullptr;
	uintptr_t pBaseAddressOfBlocks = 0;
	size_t totalSize = 0;
};

