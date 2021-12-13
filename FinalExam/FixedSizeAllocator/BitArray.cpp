#include "BitArray.h"

#include <cstring>

#include <intrin.h>
#pragma intrinsic(_BitScanReverse, _BitScanReverse64)

#include "../MemoryAlignmentHelper.h"

const size_t BitArray::sBitsPerElement = sizeof(uintptr_t) * 8; //8 bits per byte

BitArray* BitArray::Create(void* i_pBaseAddress, size_t i_numBits, bool i_bInitToZero)
{
	size_t requiredSize = GetRequiredArraySizeForBits(i_numBits);
	uintptr_t baseAddressOfBitData = reinterpret_cast<uintptr_t>(i_pBaseAddress) - requiredSize;
	uintptr_t alignedBaseAddressOfBitData = MemoryAlignmentHelper::AlignDown(baseAddressOfBitData);

	uintptr_t rootAddress = alignedBaseAddressOfBitData - sizeof(BitArray);

	BitArray* bitArray = reinterpret_cast<BitArray*>(rootAddress);
	assert(bitArray);

	//m_pBits = new uintptr_t[i_numBits / sBitsPerElement];
	return bitArray->Initialize(rootAddress, alignedBaseAddressOfBitData, i_numBits, i_bInitToZero);
}

//BitArray::~BitArray()
//{
//
//}

void BitArray::Destroy()
{
}

BitArray* BitArray::Initialize(uintptr_t i_rootAddress, uintptr_t i_alignedBaseAddressOfBitData, size_t i_numBits,	bool i_bInitToZero)
{
	pRoot = i_rootAddress;
	pBaseAddressOfBitData = i_alignedBaseAddressOfBitData;
	pBitData = reinterpret_cast<uintptr_t*>(i_alignedBaseAddressOfBitData);
	//sBaseAddressOfBitArray = i_alignedBaseAddressOfBitData;
	numBits = i_numBits;

	memset(reinterpret_cast<void*>(pBaseAddressOfBitData), i_bInitToZero ? 0 : 1, i_numBits / sBitsPerElement);

	return reinterpret_cast<BitArray*>(pRoot);
}

size_t BitArray::GetRequiredArraySizeForBits(size_t i_numBits)
{
	const size_t requiredSize = (i_numBits / sBitsPerElement);
	return requiredSize;
}

//BitArray::~BitArray()
//{
//}

void BitArray::ClearAll()
{
}

void BitArray::SetAll()
{
}

bool BitArray::AreAllBitsClear() const
{
	return false;
}

bool BitArray::AreAllBitsSet() const
{
	return false;
}

bool BitArray::IsBitSet(size_t i_bitNumber) const
{
	return false;
}

bool BitArray::IsBitClear(size_t i_bitNumber) const
{
	return false;
}

void BitArray::SetBit(size_t i_bitNumber)
{
//	assert(i_bitNumber < numBits);
//	unsigned char bit;
//#ifdef _WIN64
//	bit = _bittestandset64(&pBitData, i_bitNumber);
//#else
//	_bittestandset()
//#endif
}

void BitArray::ClearBit(size_t i_bitNumber)
{
}

bool BitArray::GetFirstClearBit(size_t& o_firstClearBitIndex) const
{
	const size_t numBytes = GetRequiredArraySizeForBits(numBits);

	size_t index = 0;
	while ((pBitData[index] == static_cast<uintptr_t>(0)) && (index < numBytes))
	{
		index++;
	}

	uintptr_t Bits = pBitData[index];

	/*size_t bit;
	for(bit = 0; bit < sBitsPerElement; bit++)
	{
		if (Bits & (1 << bit))
			break;
	}*/

//#ifdef _WIN64
//	bit = _bittestandset64(&pBitData, i_bitNumber);
//#else
//	_bittestandset()
//#endif
//
//	unsigned long bit;
//	_BitScanReverse64(&bit, Bits)
//	size_t firstClearBit = (index * sBitsPerElement) + bit;
	return false;
}

bool BitArray::GetFirstSetBit(size_t& o_firstSetBitIndex) const
{
	return false;
}

bool BitArray::operator[](size_t i_index) const
{
	return false;
}