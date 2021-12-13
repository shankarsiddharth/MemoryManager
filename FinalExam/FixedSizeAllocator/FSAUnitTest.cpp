//#define USE_HEAP_ALLOC

#include <cassert>
#include <crtdbg.h>
#include <Windows.h>

#include "BitArray.h"

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
	BitArray* newBitArray = BitArray::Create(reinterpret_cast<void*>(endAddress), 400);

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
