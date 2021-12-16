#pragma once

#include <cstdint>

class FixedSizeAllocator;
struct FSAData;

class HeapManager
{
public:
	static HeapManager* Get();
	static HeapManager* Create(void* i_pMemory, size_t i_bytes, unsigned int i_numDescriptors, FSAData* i_FSAData = nullptr, size_t i_numFSAs = 0);
	void Destroy();

	void* Alloc(size_t i_size);
	void* Alloc(size_t i_size, size_t i_align);
	void Free(void* i_pMemory);
	void Collect();
	bool Contains(void* i_pMemory) const;
	
private:
	static uintptr_t sBaseAddressOfHeapManager;

	HeapManager* Initialize(void* i_pMemory, size_t i_bytes, unsigned int i_numDescriptors, const FSAData* i_FSAData, const size_t i_numFSAs);
	FixedSizeAllocator* FindFixedSizeAllocator(size_t i_size);

	uintptr_t pRoot = 0;
	size_t totalSizeOfHeapManager = 0;
	uintptr_t pLinkedListAllocatorBaseAddress = 0;
	size_t sizeOfFixedSizeAllocatorAddressArray = 0;
	uintptr_t* pFixedSizeAllocatorBaseAddressArray = nullptr;

};

