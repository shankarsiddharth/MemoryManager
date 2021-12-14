//#define USE_HEAP_ALLOC

#include <bitset>
#include <cassert>
#include <crtdbg.h>
#include <iostream>
#include <Windows.h>

#include "BitArray.h"

int main(int i_arg, char**)
{
	size_t firstSetBitIndex;
	size_t firstClearBitIndex;

	BitArray* bitArray = new BitArray(70, true);
	bitArray->Display();
	std::cout << "" << std::endl;

	std::cout << "IsFirstClearBitIndexFound: " << bitArray->GetFirstClearBit(firstClearBitIndex) << std::endl;
	std::cout << "FirstClearBitIndex: " << firstClearBitIndex << std::endl;
	std::cout << "" << std::endl;

	std::cout << "bitArray->AreAllBitsClear(): " << bitArray->AreAllBitsClear() << std::endl;
	bitArray->Display();
	std::cout << "bitArray->AreAllBitsSet(): " << bitArray->AreAllBitsSet() << std::endl;
	bitArray->Display();
	std::cout << "" << std::endl;

	bitArray->SetAll();
	bitArray->Display();
	std::cout << "bitArray->AreAllBitsClear(): " << bitArray->AreAllBitsClear() << std::endl;
	std::cout << "" << std::endl;

	firstClearBitIndex = 0;
	std::cout << "IsFirstClearBitIndexFound: " << bitArray->GetFirstClearBit(firstClearBitIndex) << std::endl;
	std::cout << "FirstClearBitIndex: " << firstClearBitIndex << std::endl;
	std::cout << "" << std::endl;

	bitArray->SetAll();
	bitArray->Display();
	bitArray->ClearBit(33);
	bitArray->Display();
	firstClearBitIndex = 0;
	std::cout << "IsFirstClearBitIndexFound: " << bitArray->GetFirstClearBit(firstClearBitIndex) << std::endl;
	std::cout << "FirstClearBitIndex: " << firstClearBitIndex << std::endl;
	std::cout << "" << std::endl;
	std::cout << "" << std::endl;

	bitArray->Display();
	std::cout << "bitArray->AreAllBitsSet(): " << bitArray->AreAllBitsSet() << std::endl;
	bitArray->Display();
	std::cout << "" << std::endl;

	bitArray->ClearAll();
	bitArray->Display();
	std::cout << "bitArray->AreAllBitsClear(): " << bitArray->AreAllBitsClear() << std::endl;
	bitArray->Display();
	std::cout << "bitArray->AreAllBitsSet(): " << bitArray->AreAllBitsSet() << std::endl;
	bitArray->Display();
	std::cout << "" << std::endl;

	bitArray->SetBit(64);
	bitArray->Display();
	std::cout << "IsFirstSetBitFound: " << bitArray->GetFirstSetBit(firstSetBitIndex) << std::endl;
	std::cout << "FirstSetBitIndex: " << firstSetBitIndex << std::endl;
	std::cout << "" << std::endl;

	bitArray->InvertAllBits();
	bitArray->Display();
	std::cout << "" << std::endl;

	bitArray->ClearBit(64);
	bitArray->Display();
	firstSetBitIndex = 0;
	std::cout << "IsFirstSetBitFound: " << bitArray->GetFirstSetBit(firstSetBitIndex) << std::endl;
	std::cout << "FirstSetBitIndex: " << firstSetBitIndex << std::endl;
	std::cout << "" << std::endl;

	std::cout << "IsBitSet: " << bitArray->IsBitSet(64) << std::endl;
	bitArray->Display();
	bitArray->ClearBit(32);
	bitArray->Display();
	std::cout << "IsBitClear: " << bitArray->IsBitClear(64) << std::endl;
	std::cout << "" << std::endl;

	std::cout << "bitArray->AreAllBitsClear(): " << bitArray->AreAllBitsClear() << std::endl;
	bitArray->Display();
	std::cout << "bitArray->AreAllBitsSet(): " << bitArray->AreAllBitsSet() << std::endl;
	bitArray->Display();
	std::cout << "" << std::endl;

	//bitArray->SetBit(80);

	return 0;
}
