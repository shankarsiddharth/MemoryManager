#define USE_HEAP_ALLOC

#include <cassert>
#include <cinttypes>
#include <crtdbg.h>
#include <iostream>
#include <Windows.h>
#include <cstdio>

#include "BitArray.h"
#include "FixedSizeAllocator.h"

int main(int i_arg, char**)
{
	//const size_t 		sizeHeap = 1024 * 1024;
	const size_t 		sizeHeap = 1024 * 5;

	// you may not need this if you don't use a descriptor pool
	const unsigned int 	numDescriptors = 2048;

	// Allocate memory for my test heap.
#ifdef USE_HEAP_ALLOC
	void* pHeapMemory = HeapAlloc(GetProcessHeap(), 0, sizeHeap);
#else
	// Get SYSTEM_INFO, which includes the memory page size
	SYSTEM_INFO SysInfo;
	GetSystemInfo(&SysInfo);
	// round our size to a multiple of memory page size
	assert(SysInfo.dwPageSize > 0);
	size_t sizeHeapInPageMultiples = SysInfo.dwPageSize * ((sizeHeap + SysInfo.dwPageSize) / SysInfo.dwPageSize);
	void* pHeapMemory = VirtualAlloc(reinterpret_cast<LPVOID>(0x000001FF0C670000), sizeHeapInPageMultiples, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	//void* pHeapMemory = VirtualAlloc(NULL, sizeHeapInPageMultiples, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
#endif
	assert(pHeapMemory);
	//uintptr_t endAddress = reinterpret_cast<uintptr_t>(pHeapMemory) + (1024ULL * 5ULL);
	//End Address: pHeapMemory + (1024) => 0x000001FF0C670400
	//End Address: pHeapMemory + (1024 * 5) => 0x000001FF0C671400 //0x000001FF0C672000
	//0x0000016ea8955a00, 0x0000016EA8957000

	//TestCases
	uintptr_t endAddress = reinterpret_cast<uintptr_t>(pHeapMemory) + sizeHeap;
	printf("End Heap Address 0x%" PRIXPTR "\n", endAddress);

	FixedSizeAllocator* fixed_size_allocator = FixedSizeAllocator::Create(pHeapMemory, sizeHeap, 16, 8);

	void* address1 = fixed_size_allocator->Alloc();
	void* pDebugMemoryStart = address1;
	for (size_t index = 0; index < fixed_size_allocator->GetFSAData().sizeOfBlock; index++)
	{
		*static_cast<char*>(pDebugMemoryStart) = '~';
		pDebugMemoryStart = static_cast<char*>(pDebugMemoryStart) + 1;
	}

	void* address2 = fixed_size_allocator->Alloc();
	pDebugMemoryStart = address2;
	for (size_t index = 0; index < fixed_size_allocator->GetFSAData().sizeOfBlock; index++)
	{
		*static_cast<char*>(pDebugMemoryStart) = '~';
		pDebugMemoryStart = static_cast<char*>(pDebugMemoryStart) + 1;
	}

	uintptr_t addressToCheck = reinterpret_cast<uintptr_t>(address2) + fixed_size_allocator->GetFSAData().sizeOfBlock;
	void* addressToCheckPtr = reinterpret_cast<void*>(addressToCheck);
	bool isValidAddress = fixed_size_allocator->Contains(addressToCheckPtr);
	if(isValidAddress)
	{
		printf("Valid Address 0x%" PRIXPTR "\n", addressToCheck);		
	}
	else
	{
		printf("InValid Address 0x%" PRIXPTR "\n", addressToCheck);
	}
	assert(isValidAddress);

	addressToCheck = reinterpret_cast<uintptr_t>(address1) + (fixed_size_allocator->GetFSAData().sizeOfBlock * fixed_size_allocator->GetFSAData().numberOfBlocks);
	addressToCheckPtr = reinterpret_cast<void*>(addressToCheck);
	isValidAddress = fixed_size_allocator->Contains(addressToCheckPtr);
	if (isValidAddress)
	{
		printf("Valid Address 0x%" PRIXPTR "\n", addressToCheck);
	}
	else
	{
		printf("InValid Address 0x%" PRIXPTR "\n", addressToCheck);
	}
	assert(!isValidAddress);

	addressToCheck = reinterpret_cast<uintptr_t>(address1) + 8;
	addressToCheckPtr = reinterpret_cast<void*>(addressToCheck);
	if (fixed_size_allocator->Contains(addressToCheckPtr))
	{
		printf("Valid Address 0x%" PRIXPTR "\n", addressToCheck);
	}
	else
	{
		printf("InValid Address 0x%" PRIXPTR "\n", addressToCheck);
	}
	assert(!isValidAddress);

	addressToCheck = reinterpret_cast<uintptr_t>(address1) + 15;
	addressToCheckPtr = reinterpret_cast<void*>(addressToCheck);
	if (fixed_size_allocator->Contains(addressToCheckPtr))
	{
		printf("Valid Address 0x%" PRIXPTR "\n", addressToCheck);
	}
	else
	{
		printf("InValid Address 0x%" PRIXPTR "\n", addressToCheck);
	}
	assert(!isValidAddress);
	//fixed_size_allocator->Free(addressToCheckPtr);
	//fixed_size_allocator->Free(address1);
	//fixed_size_allocator->Free(address2);

	bool isMemoryFreed = fixed_size_allocator->Free(address1);
	if(isMemoryFreed)
	{
		printf("Freed Address 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(address1));
	}
	else
	{
		printf("NOT Freed Address 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(address1));
	}
	assert(isMemoryFreed);

	isMemoryFreed = fixed_size_allocator->Free(address1);
	if (isMemoryFreed)
	{
		printf("Freed Address 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(address1));
	}
	else
	{
		printf("NOT Freed Address 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(address1));
	}
	assert(!isMemoryFreed);

	if (pHeapMemory)
	{
#ifdef USE_HEAP_ALLOC
		HeapFree(GetProcessHeap(), 0, pHeapMemory);
#else
		VirtualFree(pHeapMemory, 0, MEM_RELEASE);
#endif
	}

	// in a Debug build make sure we didn't leak any memory.
#if defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

	return 0;
}
