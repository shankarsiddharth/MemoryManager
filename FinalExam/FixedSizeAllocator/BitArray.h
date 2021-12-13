#pragma once
#include <cstdint>

class BitArray
{
public:
	static BitArray* Create(void* i_pBaseAddress, size_t i_numBits, bool i_bInitToZero = true);
	//~BitArray();
	void Destroy();

	static size_t GetRequiredArraySizeForBits(size_t i_numBits);

	void ClearAll();
	void SetAll();

	bool AreAllBitsClear() const;
	bool AreAllBitsSet() const;

	inline bool IsBitSet(size_t i_bitNumber) const;
	inline bool IsBitClear(size_t i_bitNumber) const;

	void SetBit(size_t i_bitNumber);
	void ClearBit(size_t i_bitNumber);

	bool GetFirstClearBit(size_t& o_firstClearBitIndex) const;
	bool GetFirstSetBit(size_t& o_firstSetBitIndex) const;

	bool operator[](size_t i_index) const;

	static const size_t sBitsPerElement;

private:
	//static uintptr_t sBaseAddressOfBitArray;

	BitArray* Initialize(uintptr_t i_rootAddress, uintptr_t i_alignedBaseAddressOfBitData, size_t i_numBits, bool i_bInitToZero = true);

	uintptr_t pRoot = 0;
	size_t numBits = 0;
	uintptr_t pBaseAddressOfBitData = 0;
	uintptr_t* pBitData = nullptr;
};

