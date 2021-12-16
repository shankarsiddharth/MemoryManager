#include "FixedSizeAllocator.h"

#include <cstdio>

#include "MemoryAlignmentHelper.h"
#include "BitArray.h"

FixedSizeAllocator* FixedSizeAllocator::Create(void* i_pBaseAddressOfAvailableMemory, size_t i_sizeOfAvailableMemoryMemory, size_t i_sizeOfBlock, size_t i_numberOfBlocks)
{
	assert(i_numberOfBlocks);

	size_t requiredSizeForMemoryBlocks = GetRequiredSizeForFixedSizeMemoryBlocks(i_sizeOfBlock, i_numberOfBlocks);
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
	assert(isValidRootAddress && "If assert fails increase the available memory for Heap");

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

	FSAInfoData.sizeOfBlock = i_sizeOfBlock;
	FSAInfoData.numberOfBlocks = i_numberOfBlocks;

	pFSABitArray = BitArray::Create(reinterpret_cast<void*>(i_pBaseAddressOfAvailableMemory), i_remainingAvailableSize, i_numberOfBlocks, true);
	pBaseAddressOfFixedSizeMemoryBlocks = i_alignedBaseAddressOfFixedSizeMemoryBlocks;

	return reinterpret_cast<FixedSizeAllocator*>(pRoot);
}

void FixedSizeAllocator::Destroy()
{
#ifdef _DEBUG
	ShowOutstandingAllocations();
#endif
}

void* FixedSizeAllocator::Alloc()
{
	size_t i_firstAvailable;
	if(pFSABitArray->GetFirstClearBit(i_firstAvailable))
	{
		pFSABitArray->SetBit(i_firstAvailable);
		uintptr_t baseAddressOfUserMemory = pBaseAddressOfFixedSizeMemoryBlocks + (i_firstAvailable * FSAInfoData.sizeOfBlock);
		return reinterpret_cast<void*>(baseAddressOfUserMemory);
	}
	return nullptr;
}

bool FixedSizeAllocator::Free(void* i_pMemory)
{
	assert(Contains(i_pMemory));
	uintptr_t memoryAddressToCheck = reinterpret_cast<uintptr_t>(i_pMemory);
	size_t bitIndex = (memoryAddressToCheck - pBaseAddressOfFixedSizeMemoryBlocks) / FSAInfoData.sizeOfBlock;
	assert(bitIndex < FSAInfoData.numberOfBlocks);
	if(pFSABitArray->IsBitSet(bitIndex))
	{
		pFSABitArray->ClearBit(bitIndex);
		return true;
	}
	return false;
}

bool FixedSizeAllocator::Contains(void* i_pMemory) const
{
	if(i_pMemory == nullptr)
	{
		return false;
	}
	uintptr_t memoryAddressToCheck = reinterpret_cast<uintptr_t>(i_pMemory);
	uintptr_t endAddressOfFixedSizeMemoryBlocks = pBaseAddressOfFixedSizeMemoryBlocks + (FSAInfoData.numberOfBlocks * FSAInfoData.sizeOfBlock);
	bool isAddressWithinValidMemoryRange = (memoryAddressToCheck >= pBaseAddressOfFixedSizeMemoryBlocks && memoryAddressToCheck < endAddressOfFixedSizeMemoryBlocks);
	if(isAddressWithinValidMemoryRange)
	{
		//Check memoryAddressToCheck is a valid base address
		uintptr_t differenceInMemoryAddress = (memoryAddressToCheck - pBaseAddressOfFixedSizeMemoryBlocks);
		if(differenceInMemoryAddress % FSAInfoData.sizeOfBlock == 0)
		{
			return true;
		}
	}
	return false;
}

FSAData FixedSizeAllocator::GetFSAData() const
{
	return FSAInfoData;
}

size_t FixedSizeAllocator::GetRequiredSizeForFixedSizeMemoryBlocks(size_t i_sizeOfBlock, size_t i_numberOfBlocks)
{
	return i_sizeOfBlock * i_numberOfBlocks;
}

void FixedSizeAllocator::ShowFreeBlocks() const
{
	printf("FreeBlock BitArray Data for FSA of Size: %zu\n", FSAInfoData.sizeOfBlock);
	pFSABitArray->Display();
}

void FixedSizeAllocator::ShowOutstandingAllocations() const
{
	if (!pFSABitArray->AreAllBitsClear())
	{
		printf("Outstanding Allocations for FSA of Size: %zu\n", FSAInfoData.sizeOfBlock);
		pFSABitArray->Display();
	}
}
