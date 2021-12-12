#define DEBUG_MEMORY

#include "HeapManager.h"
#include <Windows.h>

#include <cstdio>
#include <iostream>
#include <iomanip>

const size_t HeapManager::sDefaultAlignment = sizeof(size_t);
uintptr_t HeapManager::sBaseAddressOfHeapManager = 0;

HeapManager* HeapManager::Get()
{
	assert(sBaseAddressOfHeapManager != 0);
	return reinterpret_cast<HeapManager*>(sBaseAddressOfHeapManager);
}

HeapManager* HeapManager::Create(void* i_pHeapMemory, size_t i_bytes, unsigned i_numDescriptors)
{
#ifdef DEBUG_MEMORY
	void* pDebugMemoryStart = i_pHeapMemory;
	printf("========================================HEAP MANAGER=======================================\n");
	printf("pHeapManager Start Address: %zX\n", reinterpret_cast<uintptr_t>(pDebugMemoryStart));
	//printf("pHeapManager End Address: %zX\n", reinterpret_cast<uintptr_t>(pDebugMemoryStart) + i_bytes);
	printf("Total Size of Heap: %zu bytes\n", i_bytes);
	printf("===========================================================================================\n");
	/*for (size_t index = 0; index < i_bytes; index++)
	{
		*static_cast<char*>(pDebugMemoryStart) = '~';
		pDebugMemoryStart = static_cast<char*>(pDebugMemoryStart) + 1;
	}*/
#endif

	auto pHeapManager = static_cast<HeapManager*>(i_pHeapMemory);
	assert(pHeapManager);

	return pHeapManager->Initialize(i_pHeapMemory, i_bytes, i_numDescriptors);
}

HeapManager* HeapManager::Create(size_t i_bytes, unsigned int i_numDescriptors)
{
	assert((i_bytes) > sizeof(HeapManager));

	SYSTEM_INFO SysInfo;
	GetSystemInfo(&SysInfo);
	// round our size to a multiple of memory page size
	assert(SysInfo.dwPageSize > 0);

	const size_t sizeHeapInPageMultiples = SysInfo.dwPageSize * ((i_bytes + SysInfo.dwPageSize) / SysInfo.dwPageSize);
	assert((sizeHeapInPageMultiples) > sizeof(HeapManager));

	void* pHeapMemory = VirtualAlloc(NULL, sizeHeapInPageMultiples, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	assert(pHeapMemory);
	
#ifdef DEBUG_MEMORY
	void* pDebugMemoryStart = pHeapMemory;
	printf("========================================HEAP MANAGER=======================================\n");
	printf("pHeapManager Start Address: %zX\n", reinterpret_cast<uintptr_t>(pDebugMemoryStart));
	//printf("pHeapManager End Address: %zX\n", reinterpret_cast<uintptr_t>(pDebugMemoryStart) + i_bytes);
	printf("Total Size of Heap: %zu bytes\n", i_bytes);
	printf("===========================================================================================\n");
	/*for (size_t index = 0; index < i_bytes; index++)
	{
		*static_cast<char*>(pDebugMemoryStart) = '~';
		pDebugMemoryStart = static_cast<char*>(pDebugMemoryStart) + 1;
	}*/
#endif

	auto pHeapManager = static_cast<HeapManager*>(pHeapMemory);
	assert(pHeapManager);

	return pHeapManager->Initialize(pHeapMemory, i_bytes, i_numDescriptors);
}

void HeapManager::Destroy()
{
	void* pHeapMemoryStart = reinterpret_cast<void*>(pRoot);
	const size_t totalSize = totalHeapSize;
	for(size_t index = 0; index < totalSize; index++)
	{
		*static_cast<char*>(pHeapMemoryStart) = NULL;
		pHeapMemoryStart = static_cast<char*>(pHeapMemoryStart) + 1;
	}
}

HeapManager* HeapManager::Initialize(void* i_pMemory, size_t i_bytes, unsigned int i_numDescriptors)
//HeapManager* HeapManager::Initialize(size_t i_bytes)
{
	sBaseAddressOfHeapManager = reinterpret_cast<uintptr_t>(i_pMemory);

	//Initialize Heap Manager
	pRoot = reinterpret_cast<uintptr_t>(i_pMemory);
	pHead = reinterpret_cast<MemoryBlock*>(pRoot + sizeof(HeapManager));
	pTail = reinterpret_cast<MemoryBlock*>(pRoot + i_bytes - sizeof(MemoryBlock));
	totalHeapSize = i_bytes;

	//Initialize Head
	pHead->pBaseAddress = reinterpret_cast<uintptr_t>(pHead);
	pHead->BlockSize = 0;
	pHead->pPreviousBlock = nullptr;
	pHead->pNextBlock = nullptr;	
	pHead->bIsAllocated = true;

	//Initialize Tail
	pTail->pBaseAddress = reinterpret_cast<uintptr_t>(pTail);
	pTail->BlockSize = 0;
	pTail->pPreviousBlock = nullptr;
	pTail->pNextBlock = nullptr;	
	pTail->bIsAllocated = true;

	//Create the InitialMemoryBlock i.e. Heap Memory Available to the User
	const size_t AvailableMemory = i_bytes - sizeof(HeapManager) - sizeof(MemoryBlock) - sizeof(MemoryBlock);
	assert(AvailableMemory > 0);	
	MemoryBlock* pNewMemoryBlock = reinterpret_cast<MemoryBlock*>(pTail->pBaseAddress - AvailableMemory);
	pNewMemoryBlock->pBaseAddress = reinterpret_cast<uintptr_t>(pNewMemoryBlock) + sizeof(MemoryBlock);
	const size_t AvailableBlockMemory = AvailableMemory - sizeof(MemoryBlock);
	assert(AvailableBlockMemory > 0);
	pNewMemoryBlock->BlockSize = AvailableBlockMemory;
	pNewMemoryBlock->pPreviousBlock = pHead;
	pNewMemoryBlock->pNextBlock = pTail;
	pNewMemoryBlock->bIsAllocated = false;

	//Link the Head, Tail & the InitialMemoryBlock
	pHead->pNextBlock = pNewMemoryBlock;
	pTail->pPreviousBlock = pNewMemoryBlock;

	return reinterpret_cast<HeapManager*>(pRoot);
}

size_t HeapManager::GetSizeRequiredForANewBlock(size_t i_size, size_t i_align)
{
	const size_t alignPadding = AlignPadding(i_size, i_align);
	//const size_t requiredSize = (2 * sizeof(MemoryBlock)) + alignPadding + i_size;
	const size_t requiredSize = sizeof(MemoryBlock) + alignPadding + i_size;
	return requiredSize;
}

MemoryBlock* HeapManager::FindFirstFit(size_t i_size, size_t i_align) const
{	
	const size_t SizeRequired = GetSizeRequiredForANewBlock(i_size, i_align);

	MemoryBlock* IteratorBlock = pTail;
	while ((nullptr != IteratorBlock))
	{
		//Find First Fit with alignment
		if(IteratorBlock->BlockSize >= SizeRequired && IteratorBlock->bIsAllocated == false)
		{
			MemoryBlock* pPotentialAvailableBlock = IteratorBlock;
			uintptr_t pNextBlockToAvailableBlockAddress = reinterpret_cast<uintptr_t>(pPotentialAvailableBlock->pNextBlock);
			//uintptr_t EndOfAvailableBlock = reinterpret_cast<uintptr_t>(pAvailableBlock) + sizeof(MemoryBlock) + pAvailableBlock->BlockSize;
			uintptr_t EndOfAvailableBlock = reinterpret_cast<uintptr_t>(pPotentialAvailableBlock->pNextBlock);
			uintptr_t PotentialUserAddress = EndOfAvailableBlock - i_size;
			uintptr_t AlignedUserAddress = AlignDown(PotentialUserAddress, i_align);
			uintptr_t StartOfNewMemoryBlock = AlignedUserAddress - sizeof(MemoryBlock);
			if (!(StartOfNewMemoryBlock < pPotentialAvailableBlock->pBaseAddress))
			{
					break;
			}
		}
		IteratorBlock = IteratorBlock->pPreviousBlock;
	}
	return IteratorBlock;
}

void HeapManager::PrintDisplayHeading() const
{
	const char separator = ' ';
	std::cout << " | ";
	std::cout << std::left << std::setw(sizeof(size_t)) << std::setfill(separator) << "MemoryBlockAt" << " | ";
	std::cout << std::left << std::setw(sizeof(size_t)) << std::setfill(separator) << "BaseAddress" << " | ";
	std::cout << std::left << std::setw(sizeof(size_t)) << std::setfill(separator) << "SizeOfBlock" << " | ";
	std::cout << std::left << std::setw(sizeof(size_t)) << std::setfill(separator) << "PreviousAddress" << " | ";
	std::cout << std::left << std::setw(sizeof(size_t)) << std::setfill(separator) << "NextAddress" << " | ";
	std::cout << std::left << std::setw(sizeof(size_t)) << std::setfill(separator) << "IsAllocated" << " | ";
	/*std::cout << std::left << std::setw(18) << std::setfill(separator) << "MemoryBlockAt" << " | ";
	std::cout << std::left << std::setw(18) << std::setfill(separator) << "BaseAddress" << " | ";
	std::cout << std::left << std::setw(18) << std::setfill(separator) << "SizeOfBlock" << " | ";
	std::cout << std::left << std::setw(18) << std::setfill(separator) << "PreviousAddress" << " | ";
	std::cout << std::left << std::setw(18) << std::setfill(separator) << "NextAddress" << " | ";
	std::cout << std::left << std::setw(11) << std::setfill(separator) << "IsAllocated" << " | ";*/
	std::cout << std::endl;
}

bool HeapManager::canCoalesceWithNextMemoryBlock(MemoryBlock* i_pMemoryBlock)
{
	if(i_pMemoryBlock == nullptr)
	{
		return false;
	}

	return !i_pMemoryBlock->bIsAllocated && i_pMemoryBlock->pNextBlock && !i_pMemoryBlock->pNextBlock->bIsAllocated;
}

MemoryBlock* HeapManager::CoalesceWithNextBlock(MemoryBlock* i_pMemoryBlock)
{
	MemoryBlock* pNextMemoryBlock = i_pMemoryBlock->pNextBlock;
	if(pNextMemoryBlock->pNextBlock != nullptr)
	{
		uintptr_t EndAddressOfNextBlock = reinterpret_cast<uintptr_t>(pNextMemoryBlock->pNextBlock);
		size_t SizeFromNextBlock = EndAddressOfNextBlock - reinterpret_cast<uintptr_t>(pNextMemoryBlock);
		uintptr_t EndAddressOfCurrentMemoryBlock = reinterpret_cast<uintptr_t>(pNextMemoryBlock);
		size_t SizeFromCurrentBlock = EndAddressOfCurrentMemoryBlock - i_pMemoryBlock->pBaseAddress;
		i_pMemoryBlock->BlockSize = SizeFromCurrentBlock + SizeFromNextBlock;
		i_pMemoryBlock->pNextBlock = pNextMemoryBlock->pNextBlock;
		i_pMemoryBlock->pNextBlock->pPreviousBlock = i_pMemoryBlock;
		void* pUserMemoryStart = reinterpret_cast<void*>(i_pMemoryBlock->pBaseAddress);
		for(size_t index = 0; index < i_pMemoryBlock->BlockSize; index++)
		{
			*static_cast<char*>(pUserMemoryStart) = NULL;
			pUserMemoryStart = static_cast<char*>(pUserMemoryStart) + 1;
		}
		return i_pMemoryBlock;
	}
	return nullptr;
}

bool HeapManager::CheckAddressWithinHeap(uintptr_t i_AddressToCheck)
{
	//std::cout << "CheckAddressWithinHeap: " << std::hex << i_AddressToCheck << std::dec << std::endl;

	if(i_AddressToCheck > pRoot && i_AddressToCheck < (pRoot+totalHeapSize))
	{
		return true;
	}
	return false;
}

void* HeapManager::Alloc(size_t i_size)
{
	return Alloc(i_size, sDefaultAlignment);
}

void* HeapManager::Alloc(size_t i_size, size_t i_align)
{
	MemoryBlock* pAvailableBlock = FindFirstFit(i_size, i_align);

	if(pAvailableBlock == nullptr)
	{
		return nullptr;
	}
		
	uintptr_t pNextBlockToAvailableBlockAddress = reinterpret_cast<uintptr_t>(pAvailableBlock->pNextBlock);

	//uintptr_t EndOfAvailableBlock = reinterpret_cast<uintptr_t>(pAvailableBlock) + sizeof(MemoryBlock) + pAvailableBlock->BlockSize;
	uintptr_t EndOfAvailableBlock = reinterpret_cast<uintptr_t>(pAvailableBlock->pNextBlock);
	uintptr_t PotentialUserAddress = EndOfAvailableBlock - i_size;
	uintptr_t AlignedUserAddress = AlignDown(PotentialUserAddress, i_align);
	uintptr_t StartOfNewMemoryBlock = AlignedUserAddress - sizeof(MemoryBlock);

	if(StartOfNewMemoryBlock < pAvailableBlock->pBaseAddress)
	{
		return nullptr;
	}

	MemoryBlock* pNewMemoryBlock = reinterpret_cast<MemoryBlock*>(StartOfNewMemoryBlock);
	pNewMemoryBlock->pBaseAddress = AlignedUserAddress;
	pNewMemoryBlock->BlockSize = i_size;
	pNewMemoryBlock->pPreviousBlock = pAvailableBlock;
	pNewMemoryBlock->pNextBlock = pAvailableBlock->pNextBlock;
	pNewMemoryBlock->bIsAllocated = true;	

	pAvailableBlock->pNextBlock = pNewMemoryBlock;

	MemoryBlock* pNextBlockToAvailableBlock = reinterpret_cast<MemoryBlock*>(pNextBlockToAvailableBlockAddress);
	assert(pNextBlockToAvailableBlock != nullptr);
	pNextBlockToAvailableBlock->pPreviousBlock = pNewMemoryBlock;

	pAvailableBlock->BlockSize = StartOfNewMemoryBlock - pAvailableBlock->pBaseAddress;

	//DisplayMemoryBlock(pNewMemoryBlock);

	assert(CheckAddressWithinHeap(pNewMemoryBlock->pBaseAddress));

	return reinterpret_cast<void*>(pNewMemoryBlock->pBaseAddress);
}

void HeapManager::Collect()
{
	MemoryBlock* IteratorBlock = pHead->pNextBlock;
	while (IteratorBlock)
	{
		if(canCoalesceWithNextMemoryBlock(IteratorBlock))
		{
			MemoryBlock* CoalescedBlock = CoalesceWithNextBlock(IteratorBlock);
			if (CoalescedBlock == nullptr)
			{
				break;
			}
			IteratorBlock = CoalescedBlock;
		}
		else
		{
			IteratorBlock = IteratorBlock->pNextBlock;
		}
	}
}

bool HeapManager::Contains(void* i_pMemory) const
{
	MemoryBlock* IteratorBlock = pTail;
	while ((nullptr != IteratorBlock))
	{		
		if (IteratorBlock->pBaseAddress == reinterpret_cast<uintptr_t>(i_pMemory))
		{
			return true;
		}
		IteratorBlock = IteratorBlock->pPreviousBlock;
	}
	return false;
}

bool HeapManager::IsAllocated(void* i_pMemory) const
{
	if (i_pMemory == nullptr)
	{
		return false;
	}

	const uintptr_t MemoryBlockAddressForUserPointer = reinterpret_cast<uintptr_t>(i_pMemory) - sizeof(MemoryBlock);
	MemoryBlock* MemoryBlockToCheck = reinterpret_cast<MemoryBlock*>(MemoryBlockAddressForUserPointer);
	assert(MemoryBlockToCheck);

	return MemoryBlockToCheck->bIsAllocated;
}

size_t HeapManager::GetLargestFreeBlock() const
{
	return 0;
}

size_t HeapManager::GetTotalFreeMemory() const
{	
	return 0;
}

void HeapManager::ShowFreeBlocks() const
{
	std::cout << "================		START	Free Blocks		================" << std::endl;
	MemoryBlock* IteratorBlock = pHead;

	if (nullptr != IteratorBlock)
	{
		PrintDisplayHeading();
	}

	while ((nullptr != IteratorBlock))
	{
		if ((IteratorBlock != pHead)
			&& (IteratorBlock != pTail)
			&& (!IteratorBlock->bIsAllocated))
		{
			DisplayMemoryBlock(IteratorBlock);
		}
		IteratorBlock = IteratorBlock->pNextBlock;
	}
	std::cout << "================		END		Free Blocks		================" << std::endl << std::endl;
}

void HeapManager::DisplayMemoryBlock(MemoryBlock* i_pMemoryBlock) const
{
	assert(i_pMemoryBlock);
	const char separator = ' ';
	uintptr_t MemoryBlockAddress = reinterpret_cast<uintptr_t>(i_pMemoryBlock);
	uintptr_t BaseAddress = i_pMemoryBlock->pBaseAddress;
	size_t SizeOfBlock = i_pMemoryBlock->BlockSize;
	uintptr_t PreviousAddress = reinterpret_cast<uintptr_t>(i_pMemoryBlock->pPreviousBlock);
	uintptr_t NextAddress = reinterpret_cast<uintptr_t>(i_pMemoryBlock->pNextBlock);
	bool IsBlockAllocated = i_pMemoryBlock->bIsAllocated;

	std::cout << " | ";	
	std::cout << std::left << std::setw(sizeof(size_t)) << std::setfill(separator) << std::hex << std::uppercase << MemoryBlockAddress << " | " << std::dec;
	std::cout << std::left << std::setw(sizeof(size_t)) << std::setfill(separator) << std::hex << std::uppercase << BaseAddress << " | " << std::dec;
	std::cout << std::left << std::setw(sizeof(size_t)) << std::setfill(separator) << SizeOfBlock << " | " << std::dec;
	std::cout << std::left << std::setw(sizeof(size_t)) << std::setfill(separator) << std::hex << std::uppercase << PreviousAddress << " | " << std::dec;
	std::cout << std::left << std::setw(sizeof(size_t)) << std::setfill(separator) << std::hex << std::uppercase << NextAddress << " | " << std::dec;
	std::cout << std::left << std::setw(sizeof(size_t)) << std::setfill(separator) << IsBlockAllocated << " | ";
	std::cout << std::endl;
}

void HeapManager::ShowOutstandingAllocations() const
{
	std::cout << "================		START	Outstanding Allocations		================" << std::endl;
	MemoryBlock* IteratorBlock = pHead;

	if (nullptr != IteratorBlock)
	{
		PrintDisplayHeading();
	}	

	while ((nullptr != IteratorBlock))
	{
		if((IteratorBlock != pHead)
			&& (IteratorBlock != pTail)
			&& (IteratorBlock->bIsAllocated))
		{
			DisplayMemoryBlock(IteratorBlock);
		}
		IteratorBlock = IteratorBlock->pNextBlock;
	}
	std::cout << "================		END		Outstanding Allocations		================" << std::endl << std::endl;
}

bool HeapManager::Free(void* i_pMemory)
{
	if(i_pMemory == nullptr)
	{
		return true;
	}

	const uintptr_t MemoryBlockAddressForUserPointer = reinterpret_cast<uintptr_t>(i_pMemory) - sizeof(MemoryBlock);
	MemoryBlock* MemoryBlockToFree = reinterpret_cast<MemoryBlock*>(MemoryBlockAddressForUserPointer);
	assert(MemoryBlockToFree);

	MemoryBlockToFree->bIsAllocated = false;
	uintptr_t AvailableBytes = reinterpret_cast<uintptr_t>(MemoryBlockToFree->pNextBlock) - MemoryBlockToFree->pBaseAddress;
	MemoryBlockToFree->BlockSize = AvailableBytes;

	return true;
}
