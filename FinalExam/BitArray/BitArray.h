#pragma once

#include <cstdint>

class BitArray
{
public:
	BitArray(size_t i_numBits, bool i_bInitToZero = true);
	~BitArray();

	static size_t GetRequiredArraySizeForBits(size_t i_numBits);

	void ClearAll();
	void SetAll();

	bool AreAllBitsClear() const;
	bool AreAllBitsSet() const;

	bool IsBitSet(size_t i_bitNumber) const;
	bool IsBitClear(size_t i_bitNumber) const;

	void SetBit(size_t i_bitNumber);
	void ClearBit(size_t i_bitNumber);

	bool GetFirstClearBit(size_t& o_firstClearBitIndex) const;
	bool GetFirstSetBit(size_t& o_firstSetBitIndex) const;

	bool operator[](size_t i_index) const;

	void Display();
	void InvertAllBits();

	static const size_t sBitsPerElement;

private:
	size_t numBits = 0;
	size_t arraySize = 0;

	uintptr_t* pBitData = nullptr;
};

