//TODO: Move Debug Definitions to a single file
//#define DEBUG_CLEAR_MEMORY_ON_DESTROY

#include "HeapManager.h"

#include <cassert>

#include "LinkedListAllocator.h"
#include "FixedSizeAllocator.h"

uintptr_t HeapManager::sBaseAddressOfHeapManager = 0;

HeapManager* HeapManager::Get()
{
	return reinterpret_cast<HeapManager*>(sBaseAddressOfHeapManager);
}

HeapManager* HeapManager::Create(void* i_pMemory, size_t i_bytes, unsigned int i_numDescriptors, FSAData* i_FSAData/* = nullptr*/, size_t i_numFSAs/* = 0*/)
{
	//Default Fixed-Size Allocators
	//FSAData iFSAData[] = { {96,8}, {32, 4}, {16, 2} };
	FSAData iFSAData[] = { {96,400}, {32, 200}, {16, 100} };
	i_numFSAs = 3;
	
	assert(i_bytes > sizeof(HeapManager));

	HeapManager* pHeapManager = static_cast<HeapManager*>(i_pMemory);
	assert(pHeapManager);

	if(i_numFSAs == 0 || i_FSAData == nullptr)
	{
		//Use Default Fixed-Size Allocators Data
		return pHeapManager->Initialize(i_pMemory, i_bytes, i_numDescriptors, iFSAData, i_numFSAs);
	}
	return pHeapManager->Initialize(i_pMemory, i_bytes, i_numDescriptors, i_FSAData, i_numFSAs);
}

HeapManager* HeapManager::Initialize(void* i_pMemory, size_t i_bytes, unsigned i_numDescriptors, const FSAData* i_FSAData, const size_t i_numFSAs)
{
	pRoot = reinterpret_cast<uintptr_t>(i_pMemory);
	sBaseAddressOfHeapManager = pRoot;
	totalSizeOfHeapManager = i_bytes;
	sizeOfFixedSizeAllocatorAddressArray = i_numFSAs;

	uintptr_t startAddressOfHeapManager = reinterpret_cast<uintptr_t>(i_pMemory);
	uintptr_t endAddressOfHeapManager = startAddressOfHeapManager + i_bytes;

	uintptr_t startAddressOfAllocators = pRoot + sizeof(HeapManager) + ((sizeOfFixedSizeAllocatorAddressArray - 1) * sizeof(uintptr_t));
	size_t availableSizeForAllocators = endAddressOfHeapManager - startAddressOfAllocators;

	for(size_t index = 0; index < sizeOfFixedSizeAllocatorAddressArray; index++)
	{
		FSAData FSAInfoData = i_FSAData[index];
		FixedSizeAllocator* FSA = FixedSizeAllocator::Create(reinterpret_cast<void*>(startAddressOfAllocators), availableSizeForAllocators, FSAInfoData.sizeOfBlock, FSAInfoData.numberOfBlocks);
		uintptr_t addressOfFSA = reinterpret_cast<uintptr_t>(FSA);
		//Make sure the rootAddress is within the available memory range
		//If assert fails increase the available memory
		assert(IsAddressWithinAvailableMemoryRange(i_pMemory, i_bytes, reinterpret_cast<void*>(addressOfFSA)) && "If assert fails increase the available memory for Heap");
		uintptr_t* pFixedSizeAllocatorBaseAddressArrayPointer = reinterpret_cast<uintptr_t*>(&pFixedSizeAllocatorBaseAddressArray + index);
		*(pFixedSizeAllocatorBaseAddressArrayPointer) = reinterpret_cast<uintptr_t>(FSA);
		availableSizeForAllocators = addressOfFSA - startAddressOfAllocators;
	}

	LinkedListAllocator* pLinkedListAllocator = LinkedListAllocator::Create(reinterpret_cast<void*>(startAddressOfAllocators), availableSizeForAllocators, i_numDescriptors);
	pLinkedListAllocatorBaseAddress = reinterpret_cast<uintptr_t>(pLinkedListAllocator);
	//Make sure the rootAddress is within the available memory range
	//If assert fails increase the available memory
	assert(IsAddressWithinAvailableMemoryRange(i_pMemory, i_bytes, reinterpret_cast<void*>(pLinkedListAllocator)) && "If assert fails increase the available memory for Heap");

	return reinterpret_cast<HeapManager*>(pRoot);
}

void HeapManager::Destroy()
{
	if (pFixedSizeAllocatorBaseAddressArray != nullptr)
	{
		for (size_t index = 0; index < sizeOfFixedSizeAllocatorAddressArray; index++)
		{
			FixedSizeAllocator* pFSA = reinterpret_cast<FixedSizeAllocator*>(*(&pFixedSizeAllocatorBaseAddressArray + index));
			pFSA->Destroy();
		}
	}
	LinkedListAllocator* pLinkedListAllocator = reinterpret_cast<LinkedListAllocator*>(pLinkedListAllocatorBaseAddress);
	pLinkedListAllocator->Destroy();

#ifdef DEBUG_CLEAR_MEMORY_ON_DESTROY
	void* pMemoryAddressStart = reinterpret_cast<void*>(pRoot);
	const size_t totalSize = totalSizeOfHeapManager;
	const size_t arraySize = totalSize / sizeof(uintptr_t);
	for (size_t index = 0; index < arraySize; index++)
	{
		*static_cast<uintptr_t*>(pMemoryAddressStart) = 0;
		pMemoryAddressStart = static_cast<uintptr_t*>(pMemoryAddressStart) + 1;
	}
#endif
	
	sBaseAddressOfHeapManager = 0;
}

void* HeapManager::Alloc(size_t i_size)
{
	void* pAllocatedMemoryAddress = nullptr;
	FixedSizeAllocator* pFSA = FindFixedSizeAllocator(i_size);

	if(pFSA)
	{
		pAllocatedMemoryAddress = pFSA->Alloc();
	}

	if(pAllocatedMemoryAddress == nullptr)
	{
		LinkedListAllocator* pLinkedListAllocator = reinterpret_cast<LinkedListAllocator*>(pLinkedListAllocatorBaseAddress);
		pAllocatedMemoryAddress = pLinkedListAllocator->Alloc(i_size);
	}

	return pAllocatedMemoryAddress;
}

void* HeapManager::Alloc(size_t i_size, size_t i_align)
{
	LinkedListAllocator* pLinkedListAllocator = reinterpret_cast<LinkedListAllocator*>(pLinkedListAllocatorBaseAddress);
	return pLinkedListAllocator->Alloc(i_size, i_align);
}

void HeapManager::Free(void* i_pMemory)
{
	if (i_pMemory == nullptr)
	{
		return;
	}
	assert(Contains(i_pMemory));

	if (pFixedSizeAllocatorBaseAddressArray != nullptr)
	{
		for (size_t index = 0; index < sizeOfFixedSizeAllocatorAddressArray; index++)
		{
			FixedSizeAllocator* pFSA = reinterpret_cast<FixedSizeAllocator*>(*(&pFixedSizeAllocatorBaseAddressArray + index));
			if (pFSA->Contains(i_pMemory))
			{
				pFSA->Free(i_pMemory);
				return;
			}
		}
	}

	LinkedListAllocator* pLinkedListAllocator = reinterpret_cast<LinkedListAllocator*>(pLinkedListAllocatorBaseAddress);
	pLinkedListAllocator->Free(i_pMemory);
}

void HeapManager::Collect()
{
	LinkedListAllocator* pLinkedListAllocator = reinterpret_cast<LinkedListAllocator*>(pLinkedListAllocatorBaseAddress);
	pLinkedListAllocator->Collect();
}

bool HeapManager::Contains(void* i_pMemory) const
{
	if (pFixedSizeAllocatorBaseAddressArray != nullptr)
	{
		for (size_t index = 0; index < sizeOfFixedSizeAllocatorAddressArray; index++)
		{
			FixedSizeAllocator* pFSA = reinterpret_cast<FixedSizeAllocator*>(*(&pFixedSizeAllocatorBaseAddressArray + index));
			if (pFSA->Contains(i_pMemory))
			{
				return true;
			}
		}
	}

	LinkedListAllocator* pLinkedListAllocator = reinterpret_cast<LinkedListAllocator*>(pLinkedListAllocatorBaseAddress);
	return pLinkedListAllocator->Contains(i_pMemory);
}


FixedSizeAllocator* HeapManager::FindFixedSizeAllocator(size_t i_size)
{
	if(pFixedSizeAllocatorBaseAddressArray == nullptr)
	{
		return nullptr;
	}

	for (size_t index = 0; index < sizeOfFixedSizeAllocatorAddressArray; index++)
	{
		FixedSizeAllocator* pFSA = reinterpret_cast<FixedSizeAllocator*>(*(&pFixedSizeAllocatorBaseAddressArray + index));
		FSAData FSAInfoData = pFSA->GetFSAData();
		if(i_size <= FSAInfoData.sizeOfBlock)
		{
			return reinterpret_cast<FixedSizeAllocator*>(*((&pFixedSizeAllocatorBaseAddressArray) + index));
		}
	}
	return nullptr;
}

bool HeapManager::IsAddressWithinAvailableMemoryRange(void* i_pMemory, size_t i_bytes, void* i_AddressToCheck)
{
	assert(i_pMemory);
	assert(i_bytes);
	assert(i_AddressToCheck);
	uintptr_t startAddressOfAvailableMemory = reinterpret_cast<uintptr_t>(i_pMemory);
	uintptr_t endAddressOfAvailableMemory = reinterpret_cast<uintptr_t>(i_pMemory) + i_bytes;
	uintptr_t addressToCheck = reinterpret_cast<uintptr_t>(i_AddressToCheck);
	bool IsAddressWithinMemory = ((addressToCheck >= startAddressOfAvailableMemory) && (addressToCheck < endAddressOfAvailableMemory));
	return IsAddressWithinMemory;
}
