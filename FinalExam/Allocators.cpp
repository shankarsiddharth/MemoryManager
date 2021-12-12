#include <inttypes.h>
#include <malloc.h>

#include <stdio.h>

#include "HeapManager.h"

void * __cdecl malloc(size_t i_size)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	printf("malloc %zu\n", i_size);
	//return _aligned_malloc(i_size, 4);
	return HeapManager::Get()->Alloc(i_size);
}

void __cdecl free(void * i_ptr)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	printf("free 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
	//return _aligned_free(i_ptr);
	HeapManager::Get()->Free(i_ptr);
}

void * operator new(size_t i_size)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	printf("new %zu\n", i_size);
	//return _aligned_malloc(i_size, 4);
	return HeapManager::Get()->Alloc(i_size);
}

void operator delete(void * i_ptr)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	printf("delete 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
	//return _aligned_free(i_ptr);
	HeapManager::Get()->Free(i_ptr);
}

void * operator new[](size_t i_size)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	printf("new [] %zu\n", i_size);
	//return _aligned_malloc(i_size, 4);
	return HeapManager::Get()->Alloc(i_size);
}

void operator delete [](void * i_ptr)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	printf("delete [] 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
	//return _aligned_free(i_ptr);
	HeapManager::Get()->Free(i_ptr);
}