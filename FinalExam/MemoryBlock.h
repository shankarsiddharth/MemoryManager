#pragma once
#include <cstdint>

struct MemoryBlock
{
	//void* pBaseAddress;
	uintptr_t pBaseAddress;
	size_t	BlockSize;	
	MemoryBlock* pPreviousBlock;
	MemoryBlock* pNextBlock;
	bool bIsAllocated;

	/*MemoryBlock()
	: pBaseAddress(0),
	BlockSize(0),
	pNextBlock(nullptr),
	pPreviousBlock(nullptr),
	bIsAllocated(false)
	{}

	::MemoryBlock& operator=(void* p_memory)
	{
		pBaseAddress = static_cast<uint8_t*>(p_memory);
		return *this;
	}*/
};
