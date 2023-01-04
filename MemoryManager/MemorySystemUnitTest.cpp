//#define USE_HEAP_ALLOC
//#define _ITERATOR_DEBUG_LEVEL 0
//
//#include <Windows.h>
//
//#include "MemorySystem.h"
//
//#include <assert.h>
//#include <algorithm>
//#include <cinttypes>
//#include <vector>
//
//#ifdef _DEBUG
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//#endif // _DEBUG
//
//bool MemorySystem_UnitTest();
//
//int main(int i_arg, char**)
//{
//	const size_t 		sizeHeap = 1024 * 1024;
//	//const size_t 		sizeHeap = 1024 * 10;
//
//	// you may not need this if you don't use a descriptor pool
//	const unsigned int 	numDescriptors = 2048;
//
//	// Allocate memory for my test heap.
//#ifdef USE_HEAP_ALLOC
//	void* pHeapMemory = HeapAlloc(GetProcessHeap(), 0, sizeHeap);
//#else
//	// Get SYSTEM_INFO, which includes the memory page size
//	SYSTEM_INFO SysInfo;
//	GetSystemInfo(&SysInfo);
//	// round our size to a multiple of memory page size
//	assert(SysInfo.dwPageSize > 0);
//	size_t sizeHeapInPageMultiples = SysInfo.dwPageSize * ((sizeHeap + SysInfo.dwPageSize) / SysInfo.dwPageSize);
//	void* pHeapMemory = VirtualAlloc(reinterpret_cast<LPVOID>(0x000001FF0C670000), sizeHeapInPageMultiples, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
//	//void* pHeapMemory = VirtualAlloc(NULL, sizeHeapInPageMultiples, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
//#endif
//
//	//uintptr_t endAddress = reinterpret_cast<uintptr_t>(pHeapMemory) + (1024ULL * 5ULL);
//	//End Address: pHeapMemory + (1024) => 0x000001FF0C670400
//	//End Address: pHeapMemory + (1024 * 5) => 0x000001FF0C671400 //0x000001FF0C672000
//	//0x0000016ea8955a00, 0x0000016EA8957000
//
//	
//		// Create your HeapManager and FixedSizeAllocators.
//		InitializeMemorySystem(pHeapMemory, sizeHeap, numDescriptors);
//
//		bool success = MemorySystem_UnitTest();
//		assert(success);
//
//		// Clean up your Memory System (HeapManager and FixedSizeAllocators)
//		DestroyMemorySystem();
//	
//
//#ifdef USE_HEAP_ALLOC
//	HeapFree(GetProcessHeap(), 0, pHeapMemory);
//#else
//	VirtualFree(pHeapMemory, 0, MEM_RELEASE);
//#endif
//
//	// in a Debug build make sure we didn't leak any memory.
//#if defined(_DEBUG)
//	_CrtDumpMemoryLeaks();
//#endif // _DEBUG
//
//	return 0;
//}
//
//
//bool MemorySystem_UnitTest()
//{
//	const size_t maxAllocations = 10 * 1024;
//	//const size_t maxAllocations = 1 * 1024;
//	std::vector<void *> AllocatedAddresses;
//
//	long	numAllocs = 0;
//	long	numFrees = 0;
//	long	numCollects = 0;
//
//	size_t totalAllocated = 0;
//
//	// reserve space in AllocatedAddresses for the maximum number of allocation attempts
//	// prevents new returning null when std::vector expands the underlying array
//	AllocatedAddresses.reserve(10 * 1024);
//	//AllocatedAddresses.reserve(1 * 1024);
//
//	// allocate memory of random sizes up to 1024 bytes from the heap manager
//	// until it runs out of memory
//	do
//	{
//		const size_t		maxTestAllocationSize = 1024;
//		//const size_t		maxTestAllocationSize = 104;
//
//		size_t			sizeAlloc = 1 + (rand() & (maxTestAllocationSize - 1));
//
//		void * pPtr = malloc(sizeAlloc);
//
//		// if allocation failed see if garbage collecting will create a large enough block
//		if (pPtr == nullptr)
//		{
//			Collect();
//
//			pPtr = malloc(sizeAlloc);
//
//			// if not we're done. go on to cleanup phase of test
//			if (pPtr == nullptr)
//				break;
//		}
//
//		AllocatedAddresses.push_back(pPtr);
//		numAllocs++;
//
//		totalAllocated += sizeAlloc;
//
//		// randomly free and/or garbage collect during allocation phase
//		const unsigned int freeAboutEvery = 0x07;
//		const unsigned int garbageCollectAboutEvery = 0x07;
//
//		if (!AllocatedAddresses.empty() && ((rand() % freeAboutEvery) == 0))
//		{
//			void * pPtrToFree = AllocatedAddresses.back();
//			AllocatedAddresses.pop_back();
//
//			free(pPtrToFree);
//			numFrees++;
//		}
//		else if ((rand() % garbageCollectAboutEvery) == 0)
//		{
//			Collect();
//
//			numCollects++;
//		}
//
//	} while (numAllocs < maxAllocations);
//
//	// now free those blocks in a random order
//	if (!AllocatedAddresses.empty())
//	{
//		// randomize the addresses
//		std::random_shuffle(AllocatedAddresses.begin(), AllocatedAddresses.end());
//
//		// return them back to the heap manager
//		while (!AllocatedAddresses.empty())
//		{
//			void * pPtrToFree = AllocatedAddresses.back();
//			AllocatedAddresses.pop_back();
//
//			delete[] pPtrToFree;
//		}
//
//		// do garbage collection
//		Collect();
//		// our heap should be one single block, all the memory it started with
//
//		// do a large test allocation to see if garbage collection worked
//		void * pPtr = malloc(totalAllocated / 2);
//
//		if (pPtr)
//		{
//			free(pPtr);
//		}
//		else
//		{
//			// something failed
//			return false;
//		}
//	}
//	else
//	{
//		return false;
//	}
//
//	// this new [] / delete [] pair should run through your allocator
//	char * pNewTest = new char[1024];
//	
//	delete[] pNewTest;
//
//	// we succeeded
//	return true;
//}
