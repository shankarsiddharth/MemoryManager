#include "FixedSizeAllocator.h"
#include "../MemoryAlignmentHelper.h"

FixedSizeAllocator* FixedSizeAllocator::Create(void* i_pBaseAddressOfAvailableMemory, size_t i_sizeOfAvailableMemoryMemory, size_t i_sizeOfBlock, size_t i_numberOfBlocks)
{
	size_t requiredSizeForMemoryBlocks = GetRequiredSizeForBlocks(i_sizeOfBlock, i_numberOfBlocks);
	uintptr_t startAddressOfAvailableMemory = reinterpret_cast<uintptr_t>(i_pBaseAddressOfAvailableMemory);
	uintptr_t endAddressOfAvailableMemory = startAddressOfAvailableMemory + i_sizeOfAvailableMemoryMemory;
	uintptr_t baseAddressOfFixedSizeMemoryBlocks = endAddressOfAvailableMemory - requiredSizeForMemoryBlocks;
	uintptr_t alignedBaseAddressOfFixedSizeMemoryBlocks = MemoryAlignmentHelper::AlignDown(baseAddressOfFixedSizeMemoryBlocks);

	size_t remainingAvailableSize = alignedBaseAddressOfFixedSizeMemoryBlocks - startAddressOfAvailableMemory;
	uintptr_t requiredSizeForBitArrayObject = BitArray::GetRequiredSizeForObject(i_pBaseAddressOfAvailableMemory, remainingAvailableSize, i_numberOfBlocks);

	uintptr_t rootAddress = alignedBaseAddressOfFixedSizeMemoryBlocks - requiredSizeForBitArrayObject - sizeof(FixedSizeAllocator);

	//Make sure the rootAddress is within the available memory range
	//If assert fails increase the available memory
	bool isValidRootAddress = (rootAddress >= startAddressOfAvailableMemory) && (rootAddress < endAddressOfAvailableMemory);
	assert(isValidRootAddress);

	FixedSizeAllocator* fixedSizeAllocator = reinterpret_cast<FixedSizeAllocator*>(rootAddress);
	assert(fixedSizeAllocator);

	size_t totalSize = endAddressOfAvailableMemory - rootAddress;
	uintptr_t baseAddressOfBitArrayObject = alignedBaseAddressOfFixedSizeMemoryBlocks - requiredSizeForBitArrayObject;

	//return fixedSizeAllocator->Initialize(rootAddress, totalSize, i_sizeOfBlock, i_numberOfBlocks, i_pBaseAddressOfAvailableMemory, remainingAvailableSize, alignedBaseAddressOfFixedSizeMemoryBlocks);
	return fixedSizeAllocator->Initialize(rootAddress, totalSize, i_sizeOfBlock, i_numberOfBlocks, baseAddressOfBitArrayObject, requiredSizeForBitArrayObject, alignedBaseAddressOfFixedSizeMemoryBlocks);
}

FixedSizeAllocator* FixedSizeAllocator::Initialize(uintptr_t i_rootAddress, size_t i_totalSize, 
	size_t i_sizeOfBlock, size_t i_numberOfBlocks, uintptr_t i_pBaseAddressOfAvailableMemory, size_t i_remainingAvailableSize, uintptr_t i_alignedBaseAddressOfFixedSizeMemoryBlocks)
{
	pRoot = i_rootAddress;
	totalSize = i_totalSize;

	//uintptr_t addressOfFSAData = pRoot + sizeof(size_t);
	//pFSAData = reinterpret_cast<FSAData*>(addressOfFSAData);
	FSAInfo.sizeOfBlock = i_sizeOfBlock;
	FSAInfo.numberOfBlocks = i_numberOfBlocks;

	pFSABlockData = BitArray::Create(reinterpret_cast<void*>(i_pBaseAddressOfAvailableMemory), i_remainingAvailableSize, i_numberOfBlocks, false);
	pBaseAddressOfFixedSizeMemoryBlocks = i_alignedBaseAddressOfFixedSizeMemoryBlocks;

	return reinterpret_cast<FixedSizeAllocator*>(pRoot);
}

void FixedSizeAllocator::Destroy()
{
}

void* FixedSizeAllocator::Alloc(size_t i_size)
{
	return nullptr;
}

bool FixedSizeAllocator::Free(void* i_pMemory)
{
	return false;
}

bool FixedSizeAllocator::Contains(void* i_pMemory)
{
	return false;
}

size_t FixedSizeAllocator::GetRequiredSizeForBlocks(size_t i_sizeOfBlock, size_t i_numberOfBlocks)
{
	return i_sizeOfBlock * i_numberOfBlocks;
}
