IMPLEMENTATION DETAILS
----------------------
	
	* Class FixedSizeAllocator	=>	Handles small memory allocations upto 96 bytes of memory
	
	* Class LinkedListAllocator	=>	Default Heap Allocator used for large memory allocations
									or when fixed size allocators runs out of memory
	
	* Class HeapManager			=>	A Manager class that chooses appropriate 
									allocator based on size and the allocators availability
	
	* Class BitArray			=>	Handles the allocation information for fixed size 
									allocators using bitdata
	

Final Exam REQUIREMENTS
-----------------------

 *  Implemented allocation function for FixedSizeAllocator - void* Alloc()	
	** Returns null if it runs out of blocks
	   - File: FinalExam\FixedSizeAllocator.cpp
	   - Line: 60
 
 
 *  Implemented freeing function for FixedSizeAllocator - bool Free(void* i_pMemory)	
	** Checks whether the address provided is a valid outstanding allocation
	   - File: FinalExam\FixedSizeAllocator.cpp
	   - Line: 72
 
 
 * FixedSizeAllocator uses BitArray class to keep track of allocated memory - BitArray* pFSABitArray
	   - File: FinalExam\FixedSizeAllocator.h
	   - Line: 33
 
 
 * FixedSizeAllocator emits Information in debug build if there are any outstanding 
   allocations while destroying
	   - File: FinalExam\FixedSizeAllocator.cpp
	   - Line: 53


 * BitArray represents an arbitrary length array of bits using an internal array
   of a native data type optimal for the platform it's built for (32- or 64-bit)
	   - File: FinalExam\BitArray.h
	   - Line: 45


 * BitArray uses the compiler intrinsics _BitScanForward() and _BitScanForward64() to 
   find the first set bit in a 32- or 64-bit value
	   - File: FinalExam\BitArray.cpp
	   - Line(s): 251 & 253

  
 * BitArray includes methods for the following,	
	
	** Finding the first set bit or clear bit
	   - File: FinalExam\BitArray.cpp
	   - Line(s): 221 & 245
	
	** A query for if a bit is set or not
	   - File: FinalExam\BitArray.cpp
	   - Line: 195 & 200
	
	** Setting or Clearing a given bit
	   - File: FinalExam\BitArray.cpp
	   - Line: 205 & 213
	
	** Setting or Clearing all the bits
	   - File: FinalExam\BitArray.cpp
	   - Line(s): 143 & 152
	
	
 *  Created FixedSizeAllocators for the following sizes and number of memory blocks
	** 0 - 16 bytes with 100 blocks.
	** 16 - 32 bytes with 200 blocks.
	** 33 - 96 bytes with 400 blocks.
	   - File: FinalExam\HeapManager.cpp
	   - Line(s): 50 to 60
	   
	   - File: FinalExam\MemorySystem.cpp 
	   - Line: 8


 *  Implemented the allocation functions in Allocators.cpp that calls the HeapManager
	to manage the allocations between FixedSizeAllocator & the default heap allocator(LinkedListAllocator)
	** malloc
	** free
	** new
	** delete
	** new[]
	** delete[]
		- File: FinalExam\HeapManager.cpp
		- Line(s): 98, 117 => (malloc, new, new[])
		- Line(s): 123 => (free, delete, delete[])


 *  Implemented ShowFreeBocks() and ShowOutstandingAllocations() for FixedSizeAllocator
		- File: FinalExam\FixedSizeAllocator.cpp
		- Line(s): 117 & 130
		

 *  Implemented indexing operator for BitArray and used compiler intrinsics _bittest64 and _bittest
		- File: FinalExam\BitArray.cpp
		- Line(s): 273, 280 & 285
