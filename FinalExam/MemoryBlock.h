#pragma once
#include <cstdint>

struct MemoryBlock
{
	uintptr_t pBaseAddress;
	size_t	BlockSize;	
	MemoryBlock* pPreviousBlock;
	MemoryBlock* pNextBlock;
	bool bIsAllocated;
};
