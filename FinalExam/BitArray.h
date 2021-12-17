#pragma once
#include <cstdint>

class BitArray
{
public:
	static BitArray* Create(void* i_pBaseAddressOfAvailableMemory, size_t i_sizeOfAvailableMemory, size_t i_numBits, bool i_bInitToZero = true);
	void Destroy();

	static size_t GetRequiredArraySizeForBits(size_t i_numBits);
	static size_t GetRequiredSizeForObject(void* i_pBaseAddressOfAvailableMemory, size_t i_sizeOfAvailableMemory, size_t i_numBits);


	size_t GetBitArraySize() const;

	bool AreAllBitsClear() const;
	bool AreAllBitsSet() const;

	bool IsBitSet(size_t i_bitNumber) const;
	bool IsBitClear(size_t i_bitNumber) const;


	bool GetFirstClearBit(size_t& o_firstClearBitIndex) const;
	bool GetFirstSetBit(size_t& o_firstSetBitIndex) const;

	bool operator[](size_t i_index) const;

	void ClearAll();
	void SetAll();

	void SetBit(size_t i_bitNumber);
	void ClearBit(size_t i_bitNumber);

	void Display();
	void InvertAllBits();

	static const size_t sBitsPerElement;

private:

	BitArray* Initialize(uintptr_t i_rootAddress, uintptr_t i_alignedBaseAddressOfBitData, size_t i_numBits, bool i_bInitToZero = true);

	uintptr_t pRoot = 0;
	size_t numBits = 0;
	size_t arraySize = 0;
	uintptr_t pBaseAddressOfBitData = 0;
	uintptr_t* pBitData = nullptr;
};
