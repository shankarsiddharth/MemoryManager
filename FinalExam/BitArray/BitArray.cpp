#include "BitArray.h"

#include <cassert>
#include <cstring>
#include <cstdio>

#include <intrin.h>
#ifdef _WIN64
#pragma intrinsic(_BitScanForward64, _bittest64)
#else
#pragma intrinsic(_BitScanForward, _bittest)
#endif

const size_t BitArray::sBitsPerElement = sizeof(uintptr_t) * 8; //8 bits per byte

BitArray::BitArray(size_t i_numBits, bool i_bInitToZero)
	: numBits(i_numBits),
	  arraySize(i_numBits / sBitsPerElement)
{
	const size_t extraBits = i_numBits % sBitsPerElement;
	if(extraBits != 0)
	{
		arraySize = (i_numBits / sBitsPerElement) + 1;
	}

	pBitData = new uintptr_t[arraySize];

#ifdef _WIN64
	const uintptr_t fillValue = (i_bInitToZero ? static_cast<uintptr_t>(0) : UINT64_MAX);
#else
	const uintptr_t fillValue = (i_bInitToZero ? static_cast<uintptr_t>(0) : UINT32_MAX);
#endif
	
	//memset(pBitData, i_bInitToZero ? 0 : 1, arraySize);

	for (size_t sizeIndex = 0; sizeIndex < arraySize; sizeIndex++)
	{
		pBitData[sizeIndex] = fillValue;
	}
}

BitArray::~BitArray()
{
}

void BitArray::Display()
{
	printf_s("pBits: ");
	for(size_t sizeIndex = 0; sizeIndex < arraySize; sizeIndex++)
	{
		uintptr_t Bits = pBitData[sizeIndex];
		//for(size_t bitIndex = 0; bitIndex < sBitsPerElement; bitIndex++)
		for (size_t bitIndex = (sBitsPerElement - 1); true; bitIndex--)
		{
			if(Bits & (static_cast<uintptr_t>(1) << bitIndex))
			{
				printf_s("1");
			}
			else
			{
				printf_s("0");
			}
			if(bitIndex == 0)
			{
				break;
			}
		}
		printf_s(" ");
	}
	printf_s("\n");
}

void BitArray::InvertAllBits()
{
	for (size_t sizeIndex = 0; sizeIndex < arraySize; sizeIndex++)
	{
		pBitData[sizeIndex] = ~(pBitData[sizeIndex]);
	}
}

size_t BitArray::GetRequiredArraySizeForBits(size_t i_numBits)
{
	const size_t requiredArraySize = (i_numBits / sBitsPerElement);
	return requiredArraySize;
}

void BitArray::ClearAll()
{
	const uintptr_t fillValue = static_cast<uintptr_t>(0);
	for (size_t sizeIndex = 0; sizeIndex < arraySize; sizeIndex++)
	{
		pBitData[sizeIndex] = fillValue;
	}
}

void BitArray::SetAll()
{
#ifdef _WIN64
	const uintptr_t fillValue = UINT64_MAX;
#else
	const uintptr_t fillValue = UINT32_MAX;
#endif
	for (size_t sizeIndex = 0; sizeIndex < arraySize; sizeIndex++)
	{
		pBitData[sizeIndex] = fillValue;
	}
}

bool BitArray::AreAllBitsClear() const
{
	const uintptr_t fillValueToTest = static_cast<uintptr_t>(0);
	for (size_t sizeIndex = 0; sizeIndex < arraySize; sizeIndex++)
	{
		if(pBitData[sizeIndex] != fillValueToTest)
		{
			return false;
		}
	}
	return true;
}

bool BitArray::AreAllBitsSet() const
{
#ifdef _WIN64
	const uintptr_t fillValueToTest = UINT64_MAX;
#else
	const uintptr_t fillValueToTest = UINT32_MAX;
#endif
	for (size_t sizeIndex = 0; sizeIndex < arraySize; sizeIndex++)
	{
		if (pBitData[sizeIndex] != fillValueToTest)
		{
			return false;
		}
	}
	return true;
}

bool BitArray::IsBitSet(size_t i_bitNumber) const
{
	return (*this)[i_bitNumber];
}

bool BitArray::IsBitClear(size_t i_bitNumber) const
{
	return !IsBitSet(i_bitNumber);
}

void BitArray::SetBit(size_t i_bitNumber)
{
	assert(i_bitNumber < numBits);
	const size_t sizeIndex = i_bitNumber / sBitsPerElement;
	const size_t bitIndex = i_bitNumber % sBitsPerElement;
	pBitData[sizeIndex] |= (static_cast<uintptr_t>(1) << bitIndex);
}

void BitArray::ClearBit(size_t i_bitNumber)
{
	assert(i_bitNumber < numBits);
	const size_t sizeIndex = i_bitNumber / sBitsPerElement;
	const size_t bitIndex = i_bitNumber % sBitsPerElement;
	pBitData[sizeIndex] &= ~(static_cast<uintptr_t>(1) << bitIndex);
}

bool BitArray::GetFirstClearBit(size_t& o_firstClearBitIndex) const
{
	unsigned long Index;
	for (size_t sizeIndex = 0; sizeIndex < arraySize; sizeIndex++)
	{
		const uintptr_t BitsToTest = ~(pBitData[sizeIndex]);
#ifdef _WIN64
		const unsigned char isNonZero = _BitScanForward64(&Index, BitsToTest);
#else
		const unsigned char isNonZero = _BitScanForward(&Index, BitsToTest);
#endif
		if (isNonZero)
		{
			o_firstClearBitIndex = (sizeIndex * sBitsPerElement) + Index;
			return true;
		}
	}
	return false;
}

bool BitArray::GetFirstSetBit(size_t& o_firstSetBitIndex) const
{
	unsigned long Index;
	for (size_t sizeIndex = 0; sizeIndex < arraySize; sizeIndex++)
	{
#ifdef _WIN64
		unsigned char isNonZero = _BitScanForward64(&Index, pBitData[sizeIndex]);
#else
		unsigned char isNonZero = _BitScanForward(&Index, pBitData[sizeIndex]);
#endif
		if (isNonZero)
		{
			o_firstSetBitIndex = (sizeIndex * sBitsPerElement) + Index;
			return true;
		}
	}
	return false;
}

bool BitArray::operator[](size_t i_index) const
{
	assert(i_index < numBits);
#ifdef _WIN64
	const __int64* MemoryBits = reinterpret_cast<__int64*>(pBitData);
	const size_t longArrayIndex = i_index / sBitsPerElement;
	const __int64 longArrayBitIndex = static_cast<__int64>(i_index) % sBitsPerElement;
	const unsigned char isBitValueEqualToOne = _bittest64(&MemoryBits[longArrayIndex], longArrayBitIndex);
#else
	const long* MemoryBits = reinterpret_cast<long*>(pBitData);
	const size_t longArrayIndex = i_index / sBitsPerElement;
	const long longArrayBitIndex = static_cast<long>(i_index) % sBitsPerElement;
	const unsigned char isBitOne = _bittest(&MemoryBits[longArrayIndex], longArrayBitIndex);
#endif
	if (isBitValueEqualToOne)
	{
		return true;
	}
	return false;
}